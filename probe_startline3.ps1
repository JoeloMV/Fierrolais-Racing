Add-Type -AssemblyName System.Drawing
$img = [System.Drawing.Bitmap]::FromFile('assets/pista.png')
$w = $img.Width
$h = $img.Height
Write-Host "size $w $h"
function IsBlack($c){ return $c.R -lt 100 -and $c.G -lt 100 -and $c.B -lt 100 }
function IsWhite($c){ return $c.R -gt 220 -and $c.G -gt 220 -and $c.B -gt 220 }
for ($y=40; $y -le 90; $y++) {
    for ($x=50; $x -le 260; $x += 4) {
        $trans = 0
        $valid = 0
        $prev = ''
        for ($dx=0; $dx -lt 40; $dx++) {
            $xx = $x + $dx
            if ($xx -ge $w) { break }
            $c = $img.GetPixel($xx, $y)
            $cur = '.'
            if (IsWhite($c)) { $cur = 'W' }
            elseif (IsBlack($c)) { $cur = 'B' }
            if ($cur -ne '.') {
                $valid++
                if ($prev -ne '' -and $cur -ne $prev) { $trans++ }
                $prev = $cur
            }
        }
        if ($valid -ge 30 -and $trans -ge 8) {
            Write-Host "cand x=$x y=$y trans=$trans valid=$valid"
        }
    }
}
$img.Dispose()
