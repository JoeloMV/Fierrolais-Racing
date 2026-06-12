Add-Type -AssemblyName System.Drawing
$img = [System.Drawing.Bitmap]::FromFile('assets/pista.png')
$w = $img.Width
$h = $img.Height
Write-Host "size $w $h"
function IsBlack($c){ return $c.R -lt 80 -and $c.G -lt 80 -and $c.B -lt 80 }
function IsWhite($c){ return $c.R -gt 220 -and $c.G -gt 220 -and $c.B -gt 220 }
$results = @()
for ($y = 50; $y -le 90; $y++) {
    for ($x = 80; $x -le 260; $x++) {
        $count = 0
        $prev = ''
        $valid = 0
        for ($dx = 0; $dx -lt 40; $dx++) {
            $xx = $x + $dx
            if ($xx -ge $w) { break }
            $c = $img.GetPixel($xx, $y)
            $char = '.'
            if (IsWhite($c)) { $char = 'W' }
            elseif (IsBlack($c)) { $char = 'B' }
            if ($char -ne '.') {
                $valid++
                if ($prev -ne '' -and $char -ne $prev) { $count++ }
                $prev = $char
            }
        }
        if ($valid -ge 20 -and $count -ge 12) {
            $results += [PSCustomObject]@{ X=$x; Y=$y; Score=$count; Valid=$valid }
        }
    }
}
$img.Dispose()
$results | Sort-Object Score -Descending | Select-Object -First 20 | ForEach-Object { Write-Host ("cand {0} {1} score {2} valid {3}" -f $_.X, $_.Y, $_.Score, $_.Valid) }
