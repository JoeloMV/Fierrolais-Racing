Add-Type -AssemblyName System.Drawing
$img = [System.Drawing.Bitmap]::FromFile('assets/pista.png')
$w = $img.Width
$h = $img.Height
Write-Host "size $w $h"
function IsBlack($c){ return $c.R -lt 50 -and $c.G -lt 50 -and $c.B -lt 50 }
function IsWhite($c){ return $c.R -gt 240 -and $c.G -gt 240 -and $c.B -gt 240 }
$top = 0
$bottom = [math]::Min($h-1, [math]::Floor($h*0.45))
for ($y=$top; $y -le $bottom; $y++) {
    $count=0
    for ($x=0; $x -lt $w-1; $x++) {
        $p = $img.GetPixel($x,$y)
        $q = $img.GetPixel($x+1,$y)
        if ((IsBlack $p -and IsWhite $q) -or (IsWhite $p -and IsBlack $q)) { $count++ }
    }
    if ($count -gt 20) { Write-Host "row $y count $count" }
}
for ($x=0; $x -lt $w; $x++) {
    $count=0
    for ($y=$top; $y -le $bottom-1; $y++) {
        $p = $img.GetPixel($x,$y)
        $q = $img.GetPixel($x,$y+1)
        if ((IsBlack $p -and IsWhite $q) -or (IsWhite $p -and IsBlack $q)) { $count++ }
    }
    if ($count -gt 20) { Write-Host "col $x count $count" }
}
$img.Dispose()
