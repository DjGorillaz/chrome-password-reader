# Chrome password reader ![Appveyor](https://ci.appveyor.com/api/projects/status/github/djgorillaz/chrome-password-reader?branch=master&svg=true)

This application exports saved passwords in Google Chrome to .csv file. Only for Windows.

###  Download [**executable file**](https://github.com/DjGorillaz/chrome-password-reader/releases/latest).

## Usage
    chrome-password-reader.exe output_file.csv

## How it works

All saved Google Chrome's passwords are stored in file «Login Data» for Windows 7/8/10. File represents SQLite database and is located in the folder «%appdata%\Local\Google\Chrome\User Data\Default».
 
Program connects to a DB using DPAPI functions, accesses encrypted fields and writes all data in a .csv file. If browser is already open, timer will be started to check the database availability.

### DPAPI
Data Protection API provides data protection by encrypting files. DPAPI encrypts files using a new password for each user, so they cannot be decrypted by another user's account. Besides that, secret key (entropy) can be used to restrict access even within one account. If entropy is not set, any program in user space can decrypt database and get original data.

It is simple to use DPAPI, because only two functions are used for encryption and decryption: CryptProtectData and CryptUnprotectData. CryptProtectData takes a pointer to structure DATA_BLOB which contains encrypted data and also the other undocumented information for decryption. If necessary, you can set entropy. Function CryptUnprotectData works similarly. It gets encrypted object and return decrypted data to application.

## License
This project is licensed under the MIT License - see the [LICENSE.md](https://github.com/DjGorillaz/chrome-password-reader/blob/master/LICENSE) file for details

## Acknowledgments

* [MSDN: Windows Data Protection](https://msdn.microsoft.com/en-us/library/ms995355.aspx)
* [Хабрахабр: Хранение паролей в Chrome](https://habrahabr.ru/post/134982/)
