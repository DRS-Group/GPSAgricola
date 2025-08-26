VARYING vec2 coord;

void MAIN()
{
    vec3 color;

    float pixelSize = tileSize / resolution;

    float xMeters = coord.x * tileSize;
    float yMeters = coord.y * tileSize;

    int pixelX = int(floor(xMeters / pixelSize));
    int pixelY = int(floor(yMeters / pixelSize));

    ivec2 texCoord = ivec2(pixelX, pixelY); // pixel coordinates

    float pixelValue = texelFetch(sprayedTex, texCoord, 0).r;
    float fieldValue = texelFetch(fieldTex, texCoord, 0).r; // 1 inside field, 0 outside

    if(fieldValue < 0.5) {
        int checkerX = int(floor(xMeters / checkerSize));
        int checkerY = int(floor(yMeters / checkerSize));
        bool isEven = mod(float(checkerX + checkerY), 2.0) < 1.0;
        color = isEven ? vec3(0.0, 0.1,  0.0) : vec3(0.0, 0.105, 0.0);
    }
    else {
        if(pixelValue == 0.0) {
            int checkerX = int(floor(xMeters / checkerSize));
            int checkerY = int(floor(yMeters / checkerSize));
            bool isEven = mod(float(checkerX + checkerY), 2.0) < 1.0;
            color = isEven ? vec3(0.0, 0.24, 0.0) : vec3(0.0, 0.27, 0.0);
        }
        else if(pixelValue < 0.45){
            color = vec3(0.0, 0.2, 0.5);
        }
        else if(pixelValue > 0.55){
            color = vec3(1.0, 0.0, 0.0);
        }
        else{
            color = vec3(0.0, 0.5, 0.0);
        }
    }

    FRAGCOLOR = vec4(color, 1.0);
}
