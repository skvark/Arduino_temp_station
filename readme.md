Arduino temperature station
===========================

This is a simple temperature monitoring station, based on Arduino and [DS18B20](http://datasheets.maxim-ic.com/en/ds/DS18B20.pdf) sensors.    

All the code isn't yet here, I'm working on it...    

- If you don't own a server, you will need Arduino, ethernet shield and a webhotel.
- If you own a server, all you need is Arduino and the code in python folder.

For web interface, see the web folder. It should be used, if you plan to use ethernet shield version.     

- [https://github.com/skvark/Arduino_temp_station/tree/master/web/backend/](https://github.com/skvark/Arduino_temp_station/tree/master/web/backend) will contain the PHP-code to save values sent by Arduino + ethernet shield
- [https://github.com/skvark/Arduino_temp_station/tree/master/web/api/](https://github.com/skvark/Arduino_temp_station/tree/master/web/api) will be done later, but it will be API to serve the temps in JSON and XML

More coming soon...     

License: [MIT](http://github.com/skvark/Arduino_temp_station/blob/master/licence.txt)     
author: Olli-Pekka Heinisuo    