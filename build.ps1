Get-ChildItem -Recurse  -Path .\ -Filter *.cpp | ForEach-Object { 
    $_.Directory.Name
}