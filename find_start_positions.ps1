Add-Type -AssemblyName System.Drawing
$img = [System.Drawing.Bitmap]::FromFile('assets/pista.png')
$w = $img.Width
$h = $img.Height
function IsBlack($c){ return $c.R -lt 80 -and $c.G -lt 80 -and $c.B -lt 80 }
function IsWhite($c){ return $c.R -gt 220 -and $c.G -gt 220 -and $c.B -gt 220 }
$results = @()
for ($y = 50; $y -le 80; $y++) {
    for ($x = 80; $x -le 260; $x++) {
        $valid = 0
        $trans = 0
        $prev = ''
        for ($dx = 0; $dx -lt 40; $dx++) {
            $xx = $x + $dx
            if ($xx -ge $w) { break }
            $c = $img.GetPixel($xx,$y)
            if (IsBlack($c)) { $cur = 'B' }
            elseif (IsWhite($c)) { $cur = 'W' }
            else { $cur = '.' }
            if ($cur -ne '.') {
                $valid++
                if ($prev -ne '' -and $cur -ne $prev) { $trans++ }
                $prev = $cur
            }
        }
        if ($valid -ge 20 -and $trans -ge 8) {
            $results += [PSCustomObject]@{X=$x; Y=$y; Trans=$trans; Valid=$valid}
        }
    }
}
$img.Dispose()
$results | Sort-Object @{Expression={$_.Trans};Descending=$true}, @{Expression={$_.Valid};Descending=$true}, @{Expression={$_.Y};Descending=$false}, @{Expression={$_.X};Descending=$false} | Select-Object -First 30 | ForEach-Object { Write-Host ("cand {0} {1} trans {2} valid {3}" -f $_.X, $_.Y, $_.Trans, $_.Valid) }
