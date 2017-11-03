# Chrome password reader

This application allows reading saved passwords in Google Chrome. Only for Windows.

## How it works

All saved Google Chrome's passwords are stored in file «Login Data» for Windows 7/8/10. File represents SQLite database and is located in the folder «%appdata%\Local\Google\Chrome\User Data\Default».
 
Program connects to a DB using DPAPI functions, accesses encrypted fields and writes all data in a .txt file. If browser is already open, timer will be started to check the database availability.

## DPAPI
Data Protection API is a cryptographic application programming interface which provides data protection by encrypting files. DPAPI encrypts files using a new password for each user, so they cannot be decrypted by another user's account. Besides that, secret key (entropy) can be used, which allows to restrict access to data even within one account. Because without knowing the secret you cannot decrypt data. If entropy is not set, any program in user space can decrypt database and get original data.

It is simple to use DPAPI, because only two functions are used for encryption and decryption: CryptProtectData and CryptUnprotectData. CryptProtectData takes a pointer to structure DATA_BLOB (Binary Large OBject), which contains not only encrypted data, but also the other undocumented information necessary for decryption. If necessary, you can set entropy. Function CryptUnprotectData woks similarly. It get encrypted object and return decrypted data to application.

## Acknowledgments

* [MSDN: Windows Data Protection](https://msdn.microsoft.com/en-us/library/ms995355.aspx)
* [Хабрахабр: Хранение паролей в Chrome](https://habrahabr.ru/post/134982/)
