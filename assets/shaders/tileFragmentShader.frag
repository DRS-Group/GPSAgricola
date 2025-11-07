VARYING vec2 coord;

void MAIN()
{
    vec3 color;

    // Convert UV to tile-local meters
    float xMeters = float(coord.x) * float(tileSize);
    float yMeters = float(coord.y) * float(tileSize);

    // Add tile world origin to make coordinates world-aligned
    float worldX = xMeters + tileWorldOrigin.x;
    float worldY = yMeters - tileWorldOrigin.y;

    // Use world-aligned coordinates for checker pattern
    int checkerX = int(floor(worldX / checkerSize));
    int checkerY = int(floor(worldY / checkerSize));
    bool isEven = mod(float(checkerX + checkerY), 2.0) < 1.0;
    color = isEven ? vec3(0.0, 0.24, 0.0) : vec3(0.0, 0.27, 0.0);

    FRAGCOLOR = vec4(color, 1.0);
}
