import requests
from bs4 import BeautifulSoup
import re
import pandas as pd
import time
from datetime import datetime
import json

def get_wikipedia_page(title, lang='bg'):
    """
    Извлича съдържание от страница в Уикипедия чрез Wikipedia API.
    """
    session = requests.Session()
    url = f"https://{lang}.wikipedia.org/w/api.php"
    params = {
        "action": "parse",
        "page": title,
        "format": "json",
        "prop": "text",
        "redirects": True
    }
    
    response = session.get(url=url, params=params)
    data = response.json()
    
    if "error" in data:
        print(f"Грешка при извличане на страницата '{title}': {data['error']['info']}")
        return None
    
    html_content = data["parse"]["text"]["*"]
    return html_content

def roman_to_arabic(roman):
    """
    Конвертира римски цифри в арабски.
    """
    roman_dict = {'I': 1, 'V': 5, 'X': 10, 'L': 50, 'C': 100, 'D': 500, 'M': 1000}
    arabic = 0
    prev_value = 0
    
    for char in reversed(roman):
        value = roman_dict.get(char, 0)
        if value >= prev_value:
            arabic += value
        else:
            arabic -= value
        prev_value = value
    
    return arabic

def extract_date_from_text(text):
    """
    Извлича дата от текста с по-сложни правила за разпознаване.
    Връща дата, оригинален текст на датата и оставащия текст.
    """
    patterns = [
        r'^(\d{1,2}\s+(?:януари|февруари|март|април|май|юни|юли|август|септември|октомври|ноември|декември)\s+\d{4}(?:\s+г\.)?)',
        r'^(\d{1,4}(?:\s?(?:г\.|година|пр\.н\.е\.|пр\. н\. е\.|н\.е\.|н\. е\.))?)[\s:–\-]',
        r'^((?:[IVX]+|[0-9]+)[\s-](?:век|в\.|столетие))',
        r'^(\d{3,4})[\s:–\-]',
        r'^(\d{3,4}\s*[-–]\s*\d{3,4}(?:\s+г\.)?)',
    ]
    
    for pattern in patterns:
        match = re.match(pattern, text)
        if match:
            date_str = match.group(1).strip()
            remaining_text = text[match.end():].strip()
            
            sort_year = None
            
            is_bc = 'пр.н.е.' in date_str or 'пр. н. е.' in date_str
            
            year_match = re.search(r'(\d{3,4})', date_str)
            if year_match:
                sort_year = int(year_match.group(1))
                if is_bc:
                    sort_year = -sort_year
            else:
                century_match = re.search(r'([IVX]+|[0-9]+)[\s-](?:век|в\.|столетие)', date_str)
                if century_match:
                    century = century_match.group(1)
                    if re.match(r'^[IVX]+$', century):
                        sort_year = roman_to_arabic(century) * 100 
                    else:
                        sort_year = int(century) * 100
            
            return date_str, remaining_text, sort_year
    
    return "Неопределена дата", text, float('inf')

def extract_events_from_page(html_content, source_page):
    """
    Извлича събития от секциите за хронология/история в страниците на Уикипедия.
    """
    soup = BeautifulSoup(html_content, "html.parser")
    events = []
    
    for ul in soup.find_all(['ul', 'ol']):
        for li in ul.find_all('li', recursive=False):
            text = li.get_text().strip()
            if not text:
                continue
            
            date_str, description, sort_year = extract_date_from_text(text)
            
            title = generate_title(date_str, description)
            location = extract_location(description, source_page)
            category = categorize_event(description)
            significance = determine_significance(description, category)
            leaders = extract_leaders(description)
            participants = extract_participants(description)
            results = extract_results(description)
            
            event = {
                'title': title,
                'description': description,
                'event_date': date_str,
                'location': location,
                'category': category,
                'significance': significance,
                'leader': leaders,
                'participants': participants,
                'results': results,
                'source': source_page.replace('_', ' '),
                'sort_year': sort_year
            }
            
            events.append(event)
    
    for p in soup.find_all('p'):
        text = p.get_text().strip()
        if len(text) > 50 and any(keyword in text.lower() for keyword in ['година', 'век', 'битка', 'война', 'въстание']):
            date_str, description, sort_year = extract_date_from_text(text)
            
            title = generate_title(date_str, description)
            location = extract_location(description, source_page)
            category = categorize_event(description)
            significance = determine_significance(description, category)
            leaders = extract_leaders(description)
            participants = extract_participants(description)
            results = extract_results(description)
            
            event = {
                'title': title,
                'description': description,
                'event_date': date_str,
                'location': location,
                'category': category,
                'significance': significance,
                'leader': leaders,
                'participants': participants,
                'results': results,
                'source': source_page.replace('_', ' '),
                'sort_year': sort_year
            }
            
            events.append(event)
    
    return events

def generate_title(date_str, description):
    """
    Генерира подходящо заглавие от датата и описанието.
    """
    if date_str and date_str != "Неопределена дата":
        short_desc = description[:50] + ('...' if len(description) > 50 else '')
        return f"{date_str} - {short_desc}"
    else:
        short_desc = description[:70] + ('...' if len(description) > 70 else '')
        return short_desc

def extract_location(description, source_page):
    """
    Извлича местоположение от текста или използва информация от източника.
    """
    location_matches = re.findall(r'(?:при|край|близо до|в района на|в околностите на|в|във|на)\s+([А-Я][а-я]+(?:\s+[А-Я][а-я]+)*)', description)
    
    if location_matches:
        return location_matches[0]
    
    bulgarian_locations = [
        'София', 'Пловдив', 'Варна', 'Бургас', 'Плиска', 'Преслав', 'Търново', 'Велико Търново',
        'Видин', 'Русе', 'Шумен', 'Стара Загора', 'България', 'Добруджа', 'Тракия', 'Мизия',
        'Родопи', 'Стара планина', 'Дунав', 'Черно море', 'Балкан', 'Константинопол', 'Цариград',
        'Македония', 'Тесалия', 'Одрин', 'Солун', 'Охрид', 'Скопие', 'Никопол'
    ]
    
    for location in bulgarian_locations:
        if location in description:
            return location
    
    if "Първа българска държава" in source_page:
        return "Първа българска държава"
    elif "Втора българска държава" in source_page:
        return "Втора българска държава"
    elif "османска власт" in source_page:
        return "България под османска власт"
    elif "Царство България" in source_page:
        return "Царство България"
    elif "Народна република" in source_page:
        return "Народна република България"
    elif "Република България" in source_page:
        return "Република България"
    elif "война" in source_page:
        war_match = re.search(r'България_(?:в|във)_(.+?)(?:_война|$)', source_page)
        if war_match:
            return f"България през {war_match.group(1).replace('_', ' ')}"
        return "Бойни действия"
    else:
        return "България"  

def categorize_event(description):
    """
    Категоризира събитието въз основа на ключови думи в описанието.
    """
    categories = {
        'Война/Битка': ['война', 'битка', 'сражение', 'обсада', 'нападение', 'превземане', 'атака', 'бой', 'поход', 'сблъсък'],
        'Политика': ['договор', 'съюз', 'споразумение', 'избори', 'парламент', 'правителство', 'конституция', 'закон', 'реформа'],
        'Управление': ['цар', 'княз', 'император', 'хан', 'крал', 'управление', 'династия', 'трон', 'коронация', 'престол', 'власт'],
        'Култура': ['култура', 'литература', 'изкуство', 'книга', 'писменост', 'азбука', 'театър', 'музика', 'наука', 'образование'],
        'Религия': ['църква', 'християнство', 'ислям', 'религия', 'манастир', 'светец', 'патриаршия', 'епархия', 'вяра'],
        'Въстание': ['въстание', 'бунт', 'революция', 'метеж', 'протест', 'недоволство', 'освобождение', 'свобода'],
        'Територия': ['граница', 'територия', 'провинция', 'област', 'присъединяване', 'анексия', 'земя'],
        'Икономика': ['икономика', 'стопанство', 'търговия', 'данък', 'пазар', 'криза', 'развитие', 'финанси']
    }
    
    text_lower = description.lower()
    
    for category, keywords in categories.items():
        if any(keyword in text_lower for keyword in keywords):
            return category
    
    if any(word in text_lower for word in ['основава', 'създаване', 'начало', 'открива']):
        return 'Основаване/Създаване'
    elif any(word in text_lower for word in ['роден', 'ражда', 'родил']):
        return 'Личности'
    elif any(word in text_lower for word in ['умира', 'смърт', 'починал']):
        return 'Личности'
    else:
        return 'Историческо събитие' 

def determine_significance(description, category):
    """
    Определя значимостта на събитието въз основа на съдържанието.
    """
    text_lower = description.lower()
    
    high_significance_indicators = [
        'значимо', 'важно', 'ключово', 'решаващо', 'историческо', 'съдбоносно', 
        'повратна точка', 'първо', 'велик', 'най-голям', 'най-важен'
    ]
    
    low_significance_indicators = [
        'малко', 'незначително', 'опит за', 'неуспешно', 'частично', 'временно'
    ]
    
    if any(indicator in text_lower for indicator in high_significance_indicators):
        return 'Висока'
    elif any(indicator in text_lower for indicator in low_significance_indicators):
        return 'Ниска'
    
    if category in ['Война/Битка', 'Въстание', 'Управление', 'Основаване/Създаване']:
        return 'Висока'
    elif category in ['Територия', 'Политика', 'Религия']:
        return 'Средна'
    else:
        return 'Средна' 

def extract_leaders(description):
    """
    Извлича имена на лидери, споменати в описанието.
    """
    famous_leaders = [
        'Аспарух', 'Тервел', 'Крум', 'Омуртаг', 'Маламир', 'Пресиян', 'Борис I', 'Симеон I', 'Петър I', 'Самуил',
        'Гаврил Радомир', 'Иван Владислав', 'Пресиян II', 'Петър II Делян', 'Константин Бодин', 'Иван Асен I', 
        'Теодор Петър', 'Калоян', 'Борил', 'Иван Асен II', 'Коломан I Асен', 'Михаил II Асен', 'Коломан II Асен', 
        'Мицо Асен', 'Константин Тих', 'Йоан Асен III', 'Иван Смилец', 'Чака', 'Теодор Светослав', 'Георги I Тертер', 
        'Михаил III Шишман', 'Иван Стефан', 'Иван Александър', 'Иван Шишман', 'Иван Срацимир', 'Фружин', 'Константин II Асен', 
        'Георги Раковски', 'Васил Левски', 'Христо Ботев', 'Любен Каравелов', 'Захари Стоянов', 'Стефан Стамболов', 
        'Панайот Волов', 'Георги Бенковски', 'Тодор Каблешков', 'Хаджи Димитър', 'Стефан Караджа', 'Филип Тотю', 
        'Сава Муткуров', 'Александър Батенберг', 'Константин Стоилов', 'Димитър Петков', 'Тодор Бурмов', 'Петко Каравелов', 
        'Васил Радославов', 'Никола Генадиев', 'Стилиян Костов', 'Димитър Греков', 'Андрей Ляпчев', 'Иван Евстатиев Гешов', 
        'Александър Малинов', 'Борис III', 'Фердинанд I', 'Кимон Георгиев', 'Георги Кьосеиванов', 'Добри Божилов', 
        'Иван Багрянов', 'Константин Муравиев', 'Георги Димитров', 'Васил Коларов', 'Вълко Червенков', 'Тодор Живков', 
        'Димитър Попов', 'Жан Виденов', 'Иван Костов', 'Симеон Сакскобургготски', 'Сергей Станишев', 'Бойко Борисов', 
        'Пламен Орешарски', 'Румен Радев', 'Кирил Петков', 'Гроздан Караджов', 'Александър Цанков', 'Никола Мушанов', 
        'Николай Добрев', 'Петър Младенов', 'Росен Плевнелиев', 'Огнян Герджиков', 'Даниел Вълчев', 'Николай Василев', 
        'Христо Иванов', 'Красимир Каракачанов', 'Волен Сидеров', 'Мустафа Карадайъ', 'Делян Пеевски', 'Слави Трифонов', 
        'Костадин Костадинов', 'Веселин Марешки', 'Лиляна Павлова', 'Меглена Кунева', 'Петър Стоянов', 'Желю Желев', 
        'Трайчо Трайков', 'Йорданка Фандъкова', 'Бойко Рашков', 'Николай Денков', 'Асен Василев', 'Стефан Янев', 
        'Тодор Александров', 'Александър Протогеров', 'Иван Михайлов', 'Дамян Велчев', 'Владимир Стойчев', 
        'Димитър Благоев', 'Антон Югов', 'Георги Дамянов', 'Станко Тодоров', 'Богдан Филов', 'Петър Дертлиев', 
        'Стефан Савов', 'Атанас Семерджиев', 'Георги Атанасов', 'Марин Дринов', 'Димитър Пешев', 'Никола Петков', 
        'Радко Димитриев', 'Данаил Николаев', 'Стилиян Ковачев', 'Иван Фичев', 'Георги Тодоров', 'Климент Бояджиев', 
        'Никола Иванов', 'Владимир Вазов', 'Христо Луков', 'Иван Колев', 'Никола Жеков', 'Атанас Семерджиев', 
        'Добри Джуров', 'Стою Неделчев-Чочоолу', 'Борис Дрангов', 'Георги Измирлиев', 'Иларион Драгостинов', 
        'Петко войвода', 'Гоце Делчев', 'Даме Груев', 'Яне Сандански', 'Христо Татарчев', 'Григор Пърличев', 
        'Кузман Шапкарев', 'Тодор Кантарджиев', 'Капитан Петко войвода', 'Мара Бунева', 'Христо Чернопеев', 
        'Пейо Яворов', 'Тодор Паница', 'Михаил Герджиков', 'Петър Чаулев', 'Александър Протогеров', 
        'Петър Дънов', 'Светослав Тертер', 'Ивайло', 'Иванко', 'Момчил войвода', 'Станислав Петрович', 
        'Гаврил Кръстевич', 'Филип Македонски (връзка с Одриското царство)', 'Севт III', 'Спартак (спорно, но често споменаван)',
        'Райна Княгиня', 'Стефан Стамболийски', 'Александър Греков', 'Крум Лекарски', 'Иван Кирков', 
        'Стоян Заимов', 'Димитър Общи', 'Иван Орлински', 'Лев Главинчев', 'Никола Габровски'
    ]

    
    title_patterns = [
        r'(цар\s+[А-Я][а-я]+(?:\s+[А-Я][а-я]+)*)',
        r'(княз\s+[А-Я][а-я]+(?:\s+[А-Я][а-я]+)*)',
        r'(хан\s+[А-Я][а-я]+(?:\s+[А-Я][а-я]+)*)',
        r'(император\s+[А-Я][а-я]+(?:\s+[А-Я][а-я]+)*)',
        r'(крал\s+[А-Я][а-я]+(?:\s+[А-Я][а-я]+)*)',
        r'(генерал\s+[А-Я][а-я]+(?:\s+[А-Я][а-я]+)*)',
        r'(министър[\s-]председател\s+[А-Я][а-я]+(?:\s+[А-Я][а-я]+)*)',
        r'(водач\s+[А-Я][а-я]+(?:\s+[А-Я][а-я]+)*)'
    ]
    
    found_leaders = []
    
    for leader in famous_leaders:
        if leader in description:
            found_leaders.append(leader)
    
    for pattern in title_patterns:
        matches = re.findall(pattern, description)
        found_leaders.extend(matches)
    
    name_pattern = r'(?<![А-Я])[А-Я][а-я]+(?:\s+[А-Я][а-я]+){1,2}(?!\s+[А-Я])'
    names = re.findall(name_pattern, description)
    
    for name in names:
        if name not in found_leaders and any(title in description.lower() + " " + name.lower() for title in 
                                            ['водач', 'лидер', 'командва', 'управлява', 'ръководи']):
            found_leaders.append(name)
    
    if found_leaders:
        return ', '.join(found_leaders)
    else:
        countries = ['България', 'Византия', 'Османска империя', 'Русия', 'Сърбия', 'Румъния', 'Гърция']
        for country in countries:
            if country in description:
                return f"Лидери на {country}"
        return "Неуточнени лидери"

def extract_participants(description):
    """
    Извлича основните участници в събитието.
    """
    participant_patterns = [
        r'(българск[а-я]+\s+(?:войск|арми)[а-я]+)',
        r'(османск[а-я]+\s+(?:войск|арми)[а-я]+)',
        r'(византийск[а-я]+\s+(?:войск|арми)[а-я]+)',
        r'(руск[а-я]+\s+(?:войск|арми)[а-я]+)',
        r'(въстаниц[а-я]+)',
        r'(четниц[а-я]+)',
        r'(опълченц[а-я]+)',
        r'(българск[а-я]+\s+(?:народ|население)[а-я]*)',
        r'(селян[а-я]+)',
        r'(граждан[а-я]+)'
    ]
    
    countries_and_peoples = [
        'България', 'Византия', 'Османска империя', 'Русия', 'Сърбия', 'Румъния', 'Гърция',
        'българи', 'византийци', 'османци', 'турци', 'руснаци', 'сърби', 'румънци', 'гърци',
        'Австро-Унгария', 'Германия', 'Франция', 'Великобритания', 'Съветски съюз'
    ]
    
    found_participants = []
    
    for pattern in participant_patterns:
        matches = re.findall(pattern, description)
        found_participants.extend(matches)
    
    for entity in countries_and_peoples:
        if entity in description:
            found_participants.append(entity)
    
    if found_participants:
        unique_participants = list(set(found_participants))
        return ', '.join(unique_participants)
    else:
        if 'война' in description.lower() or 'битка' in description.lower():
            return "Военни сили"
        elif 'въстание' in description.lower():
            return "Въстаници и османски сили"
        elif 'договор' in description.lower() or 'споразумение' in description.lower():
            return "Дипломатически представители"
        elif 'култура' in description.lower() or 'изкуство' in description.lower():
            return "Културни дейци"
        else:
            return "Български народ"

def extract_results(description):
    """
    Извлича резултатите или последствията от събитието.
    """
    result_patterns = [
        r'(?:в резултат|в следствие|като резултат)(?:[^.]*)',
        r'(?:довежда до|води до|довел до)(?:[^.]*)',
        r'(?:последица|последствие)(?:[^.]*)',
        r'(?:завършва с|приключва с)(?:[^.]*)'
    ]
    
    for pattern in result_patterns:
        results = re.findall(pattern, description, re.IGNORECASE)
        if results:
            return results[0].strip()
    
    text_lower = description.lower()
    
    if 'победа' in text_lower:
        sides = re.findall(r'победа\s+(?:на|за)\s+([^.]*)', text_lower)
        if sides:
            return f"Победа за {sides[0].strip()}"
        return "Военна победа"
    
    elif 'поражение' in text_lower:
        sides = re.findall(r'поражение\s+(?:на|за)\s+([^.]*)', text_lower)
        if sides:
            return f"Поражение за {sides[0].strip()}"
        return "Военно поражение"
    
    elif 'подписан' in text_lower and ('договор' in text_lower or 'споразумение' in text_lower):
        return "Подписан дипломатически документ"
    
    elif 'създаден' in text_lower or 'основан' in text_lower:
        created = re.findall(r'(?:създаден|основан)[а-я]*\s+(?:е|са)\s+([^.]*)', text_lower)
        if created:
            return f"Създаване на {created[0].strip()}"
        return "Създаване на нова институция/организация"
    
    elif 'присъединен' in text_lower:
        return "Териториални промени"
    
    elif 'коронован' in text_lower or 'възкачва' in text_lower:
        return "Промяна в управлението"
    
    elif 'умира' in text_lower or 'смърт' in text_lower:
        return "Смърт на историческа личност"
    
    else:
        if 'война' in text_lower or 'битка' in text_lower:
            return "Военен конфликт с геополитически последици"
        elif 'въстание' in text_lower:
            return "Политически и социални последици"
        elif 'реформа' in text_lower:
            return "Обществени и институционални промени"
        elif 'закон' in text_lower:
            return "Правни и административни промени"
        else:
            return "Исторически значими последици за българската държава"

def scrape_bulgarian_history():
    """
    Основна функция за извличане на български исторически събития от Уикипедия.
    """
    pages = [
        "История_на_България",
        "Първа_българска_държава",
        "Втора_българска_държава",
        "Средновековна_България",
        "България_под_османска_власт",
        "Българско_възраждане",
        "Княжество_България",
        "Царство_България",
        "Народна_република_България",
        "България_в_Първата_световна_война",
        "България_във_Втората_световна_война",
        "Република_България",
        "Българско-византийски_войни",
        "Априлско_въстание",
        "Балкански_войни",
        "Съединение_на_България",
        "Българска_екзархия",
        "Старобългарска_литература",
        "Османско_владичество"
    ]
    
    all_events = []
    
    for page in pages:
        print(f"Извличане на данни от страница: {page}")
        html_content = get_wikipedia_page(page, lang='bg')
        
        if html_content:
            events = extract_events_from_page(html_content, page)
            all_events.extend(events)
            
            print(f"  Намерени {len(events)} събития")
            
            time.sleep(1)
    
    df = pd.DataFrame(all_events)
    
    if not df.empty and 'description' in df.columns:
        df = df.drop_duplicates(subset=['description'], keep='first')
    
        if 'sort_year' in df.columns:
            df = df.sort_values('sort_year')
            df = df.drop('sort_year', axis=1)
    
    timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
    filename = f"български_исторически_събития_{timestamp}.csv"
    df.to_csv(filename, index=False, encoding='utf-8')
    print(f"Запазени {len(df)} събития в {filename}")
    
    json_filename = f"български_исторически_събития_{timestamp}.json"
    df.to_json(json_filename, orient='records', force_ascii=False, indent=4)
    print(f"Запазени данни в {json_filename}")
    
    return df

if __name__ == "__main__":
    print("Стартиране на скрейпър за български исторически събития от Уикипедия...")
    result = scrape_bulgarian_history()
    print(f"Извличането е завършено. Намерени са {len(result)} уникални исторически събития.")