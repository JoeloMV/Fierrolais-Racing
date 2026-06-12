Add-Type -AssemblyName System.Drawing
$img = [System.Drawing.Bitmap]::FromFile('assets/pista.png')
$w = $img.Width
$h = $img.Height
Write-Host ("size {0} {1}" -f $w, $h)
function IsBlack($c){ return $c.R -lt 100 -and $c.G -lt 100 -and $c.B -lt 100 }
function IsWhite($c){ return $c.R -gt 220 -and $c.G -gt 220 -and $c.B -gt 220 }
$results = @()
for ($x=50; $x -le 260; $x++) {
    $trans = 0
    $prev = ''
    $valid = 0
    $startY = -1
    for ($y=40; $y -le 90; $y++) {
        $c = $img.GetPixel($x, $y)
        $cur = '.'
        if (IsWhite($c)) { $cur = 'W' }
        elseif (IsBlack($c)) { $cur = 'B' }
        if ($cur -ne '.') {
            if ($startY -lt 0) { $startY = $y }
            $valid++
            if ($prev -ne '' -and $cur -ne $prev) { $trans++ }
            $prev = $cur
        }
    }
    if ($valid -gt 15 -and $trans -gt 6) {
        $results += [PSCustomObject]@{X=$x;Trans=$trans;Valid=$valid;StartY=$startY}
    }
}
$img.Dispose()
$results | Sort-Object Trans -Descending | Select-Object -First 20 | ForEach-Object { Write-Host ("col {0} trans {1} valid {2} startY {3}" -f $_.X, $_.Trans, $_.Valid, $_.StartY) }
