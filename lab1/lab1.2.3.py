# -*- coding: cp1251 -*-
#-*- coding: utf8 -*-
#AlexChib lab1,2,3
from grab import Grab
import urllib2
import urlparse
from BeautifulSoup import BeautifulSoup
from os.path import basename
import os
from urlparse import urlsplit
from BeautifulSoup import BeautifulSoup


def GoSave(url): #на входе из файла "start" принимаем url
    name = '' #для новоиспеченного каталога, содержащего содержимое данной страницы, нам нужно имя
    for i in range(0, len(url)): #делаем имя, используя саму ссылку.
        if url[i].isalpha(): 
            name += url[i]   
    if 'http' in name:       
        name = name[4:]      
    elif 'https' in name:    
        name = name[5:]      
    try: #Пытаемся пройти по ссылке и сделать всё необходимое...
        url_content = urllib2.urlopen(url).read()   #Получаем исходники
        soup = BeautifulSoup(''.join(url_content))  #^
        image_tags = soup.findAll('img') # Ищем все теги "img", получаем на выходе список фрагментов исходника, содержащий ссылки
        js_tags = soup.findAll('script') #Ищем все теги "script"
        css_tags = soup.findAll('link')#Ищем все теги "link"
        try: #Пытаемся создать новый каталог, названный $name$.
            os.mkdir(name) #^
        except: #Если такой каталог уже есть, забываем про это дело.
            pass #^
        os.chdir(name) #Следуем в эту папку.
        f = open('%s.html' % name, 'w') #Сохраняем страницу, названную $name$.
        if len(image_tags) != 0 or len(js_tags) != 0 or len(css_tags): #Если списки с нужными объектами не пусты, идём дальше.
            g = Grab() #Создаем объект Grab
            for image_tag in image_tags: #Пробегаемся по элементам списка
                try: #Пытаемся следовать по ссылке
                    image_url = image_tag['src']  #Из n-го элемента списка со ссылками вычленяем нужное
                    g.go(image_url) #                
                    imageName = basename(urlsplit(image_url)[2]) #Сплитим нашу ссылку, приобретая имя файла и его расширение                    
                    g.response.save(r'images\%s' % imageName) #Сохраняем на диск в папку images.
                    image_url = str(image_url)
                    imageName = str(imageName)
                    url_content = url_content.replace(image_url, './images/' + imageName)# 42
                    
                except:
                    pass
            for js_tag in js_tags: #Пробегаемся циклом for по элементам списка.
                try:
                    js_url = js_tag['src'] #Используя аттрибут src, берём ссылку в чистом виде
                    g.go(js_url) #Следуем по ссылке.
                    jsName = basename(urlsplit(js_url)[2]) #Сплитим её, приобретая расширение и имя файла.
                    g.response.save(r'js\%s' % jsName) #Сохраняем в папке js.
                    js_url = str(js_url)
                    jsName = str(jsName)
                    url_content = url_content.replace(js_url, './js/' + jsName)  #51                     
                except:
                    pass
                
            for css_tag in css_tags: #Пробегаемся циклом for по элементам списка.            
                try:
                    css_url = css_tag['href'] #Используя аттрибут href, берём ссылку в чистом виде
                    g.go(css_url) #Следуем по ссылке.
                    cssName = basename(urlsplit(css_url)[2]) #Сплитим её, приобретая расширение и имя файла.
                    g.response.save(r'css\%s' % cssName) #Сохраняем в папке css.
                    css_url = str(css_url)
                    cssName = str(cssName)
                    url_content = url_content.replace(css_url, './css/' + cssName) #61
                except:
                    pass 
        f.write(url_content)     
        f.close()           #Сохранили.
        os.chdir('../') #Переходим на уровень выше.
    except: 
        pass


def CreateElementsOfLink(url, old_links):
    try:
        g = Grab() #Создаём объект Грэб. 
        url_content = urllib2.urlopen(url).read() #Формируем исходник
        soup = BeautifulSoup(''.join(url_content)) #^
        url_tags = soup.findAll('a') # Ищем все теги "a"
        for url_tag in url_tags: #Проходимся циклом for по элементам новоиспеченного списка, который содержит ссылки
            try: #Пытаемся
                url_ofFind = url_tag.get('href') #Из кода, содержащего ссылки, с помощью аттрибута href выделяем ссылки явным видом
                url_ofFind = urlparse.urljoin(url, url_ofFind) #Преобразуем относительные пути в абсолютные
                g.go(url_ofFind) #Переходим по ссылке.
                if g.doc.text_search(u'МОДЕЛИ АДАПТИВНОГО ОБУЧЕНИЯ В КОМПЬЮТЕРНЫХ СИСТЕМАХ'): #Если на данном этапе, в тексте страницы имеется то, что нам нужно, то
                    #Запрос к процедуре модуль-файла, отправляющий наш урл.
                    GoSave(url_ofFind)
            except: #Перестаём пытаться, если что-то пошло не так.
                pass #^
    except:
        pass
    ChangeTheLink(url_tags, old_links)


def ChangeTheLink(urls, old_links):
    for elem in urls:
        if elem not in old_links:
            try:
                CreateElementsOfLink(elem, old_links)
            except:
                pass
            old_links.append(elem)
     
old_links = [] 
CreateElementsOfLink("http://www.rae.ru/snt/?section=content&op=show_article&article_id=6154", old_links)
