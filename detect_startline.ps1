Add-Type -AssemblyName System.Drawing
$img = [System.Drawing.Bitmap]::FromFile('assets/pista.png')
$w = $img.Width
$h = $img.Height
Write-Host "size $w $h"
function IsBlack($c){ return $c.R -lt 100 -and $c.G -lt 100 -and $c.B -lt 100 }
function IsWhite($c){ return $c.R -gt 220 -and $c.G -gt 220 -and $c.B -gt 220 }
$best = @()
for ($y=50; $y -le 80; $y++) {
    for ($x=50; $x -le 260; $x++) {
        $trans = 0
        $valid = 0
        $prev = ''
        for ($dx=0; $dx -lt 60; $dx++) {
            $xx = $x + $dx
            if ($xx -ge $w) { break }
            $c = $img.GetPixel($xx, $y)
            $cur = '.'
            if (IsBlack($c)) { $cur = 'B' }
            elseif (IsWhite($c)) { $cur = 'W' }
            if ($cur -eq '.') { continue }
            $valid++
            if ($prev -ne '' -and $cur -ne $prev) { $trans++ }
            $prev = $cur
        }
        if ($valid -gt 20 -and $trans -ge 6) {
            $best += [PSCustomObject]@{X=$x; Y=$y; Trans=$trans; Valid=$valid}
        }
    }
}
$img.Dispose()
$best | Sort-Object -Property Trans -Descending, Valid -Descending | Select-Object -First 30 | ForEach-Object { Write-Host ("cand {0} {1} trans {2} valid {3}" -f $_.X, $_.Y, $_.Trans, $_.Valid) }
