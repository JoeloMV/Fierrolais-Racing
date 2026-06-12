Add-Type -AssemblyName System.Drawing
$img = [System.Drawing.Bitmap]::FromFile('assets/pista.png')
$w = $img.Width
$h = $img.Height
function IsBlack($c){ return $c.R -lt 80 -and $c.G -lt 80 -and $c.B -lt 80 }
function IsWhite($c){ return $c.R -gt 220 -and $c.G -gt 220 -and $c.B -gt 220 }
Write-Host "size $w $h"
for ($y=30; $y -le 90; $y++) {
    $bestScore = 0
    $bestX = -1
    for ($x=0; $x -le 260; $x++) {
        $score = 0
        $last = ' '
        for ($dx=0; $dx -lt 40; $dx++) {
            if ($x + $dx -ge $w) { break }
            $c = $img.GetPixel($x+$dx, $y)
            $current = '.'
            if (IsWhite($c)) { $current = 'W' }
            elseif (IsBlack($c)) { $current = 'B' }
            if ($current -eq '.') { continue }
            if ($last -ne ' ' -and $current -ne $last) { $score++ }
            $last = $current
        }
        if ($score -gt $bestScore) { $bestScore = $score; $bestX = $x }
    }
    if ($bestScore -gt 5) { Write-Host ("row {0} bestX {1} score {2}" -f $y, $bestX, $bestScore) }
}
$img.Dispose()
