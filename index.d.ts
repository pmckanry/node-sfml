interface Buffer extends Uint8Array { }

declare interface WindowOptions {
    title: string;
    style: number;
    mode?: {
        width?: number;
        height?: number;
        bitsPerPixel?: number;
    }
}

declare interface ContextSettings {
    depthBits: number;
    stencilBits: number;
    antialiasingLevel: number;
    majorVersion: number;
    minorVersion: number;
    attributeFlags: number;
    sRgbCapable: number;
}

declare interface Size {
    w: number;
    h: number;
}

declare interface Point2D {
    x: number;
    y: number;
}

declare interface Point3D {
    x: number;
    y: number;
    z: number;
}

declare interface Rect {
    x: number;
    y: number;
    w: number;
    h: number;
}

declare interface Color {
    r: number;
    g: number;
    b: number;
    a: number;
}

export class Window {
    position: Point2D;
    size: Size;

    constructor(options: WindowOptions);
    close(): void;
    destroy(): void;
    isOpen(): boolean;
    getSettings(): ContextSettings;
    pollEvent(): void;
    waitEvent(): void;
    setTitle(title: string): void;
    setIcon(size: Size, pixel: Buffer);
    setVisible(visible: boolean): void;
    setVerticalSyncEnabled(enabled: boolean): void;
    setMouseCursorVisible(visible: boolean): void;
    setMouseCursorGrabbed(grabbed: boolean): void;
    setKeyRepeatEnabled(enabled: boolean): void;
    setFramerateLimit(limit: number): void;
    setJoystickThreshold(threshold: number): void;
    setActive(active: boolean): void;
    requestFocus(): void;
    hasFocus(): boolean;
    display(): void;
}

export class View {
    center: Point2D;
    size: Size;
    rotation: number;
    viewport: Rect;

    constructor(area?: Rect);
    copy(): View;
    destroy(): void;
    move(point: Point2D): void;
    reset(area: Rect): void;
    rotate(angle: number): void;
    zoom(factor: number): void;
}

export class Image {
    constructor(size: Size);
    copy(): Image;
    destroy(): void;
    saveToFile(filename: string): boolean;
    getSize(): Size;
    createMaskFromColor(mask: Color, alpha?: number): void;
    setPixel(point: Point2D, color: Color): void;
    getPixel(point: Point2D): Color;
    flipHorizontally(): void;
    flipVertically(): void;

    static createFromColor(area: Size, color: Color): Image;
    static createFromPixels(area: Size, pixels: Buffer): Image;
    static createFromFile(filename: string): Image;
    static createFromMemory(data: Buffer): Image;
}

export class Texture {
    smooth: boolean;
    sRGB: boolean;
    repeated: boolean;

    constructor(size: Size);
    bind(): void;
    copy(): Texture;
    copyToImage(): Image;
    destroy(): void;
    generateMipMap(): boolean;
    getNativeHandle(): number;
    getSize(): Size;
    updateFromPixels(pixels: Buffer, rect: Rect): void;
    updateFromImage(image: Image, point: Point2D): void;
    updateFromWindow(window: Window, point: Point2D): void;
    updateFromRenderWindow(renderWindow: RenderWindow, point: Point2D): void;

    static createFromFile(filename: string, area?: Rect): Texture;
    static createFromMemory(buffer: Buffer): Texture;
    static getMaximumSize(): number;
}

export class Sprite {
    position: Point2D;
    rotation: number;
    scale: Point2D;
    origin: Point2D;
    texture: Texture;
    textureRect: Rect;
    color: Color;

    constructor();
    copy(): Sprite;
    destroy(): void;
    move(point: Point2D): void;
    rotate(angle: number): void;
    scaleTo(factor: Point2D): void;
    getTransform(): Array<number>;
    getInverseTransform(): Array<number>;
    getLocalBounds(): Rect;
    getGlobalBounds(): Rect;
}

export class RenderWindow {
    position: Point2D;
    size: Size;
    view: View;
    mousePosition: Point2D;

    constructor(options: WindowOptions);
    close(): void;
    destroy(): void;
    isOpen(): boolean;
    getSettings(): ContextSettings;
    pollEvent(): void;
    waitEvent(): void;
    setTitle(title: string): void;
    setIcon(size: Size, pixels: Buffer): void;
    setVisible(visible: boolean): void;
    setVerticalSyncEnabled(enabled: boolean): void;
    setMouseCursorVisible(visible: boolean): void;
    setMouseCursorGrabbed(grabbed: boolean): void;
    setKeyRepeatEnabled(enabled: boolean): void;
    setFramerateLimit(limit: number): void;
    setJoystickThreshold(threshold: number): void;
    setActive(active: boolean): void;
    requestFocus(): void;
    hasFocus(): boolean;
    display(): void;
    clear(color: Color): void;
    getDefaultView(): View;
    getViewport(view: View): Rect;
    mapPixelToCoordinates(point: Point2D, view: View): Point2D;
    mapCoordinatesToPixel(point: Point2D, view: View): Point2D;
    drawSprite(sprite: Sprite): void;
    /*
    drawText(text: Text): void;
    drawShape(shape: Shape): void;
    drawCircleShape(circle: CircleShape): void;
    drawConvexShape(convex: ConvexShape): void;
    drawRectangleShape(shape: RectangleShape): void;
    drawVertexArray(arr: VertexArray): void;
    drawPrimitives(arr: Primitives): void;
    */
    pushGLStates(): void;
    popGLStates(): void;
    resetGLStates(): void;
    getTouchPosition(finger: number): Point2D;
}

export class Clock {
    constructor();
    copy(): Clock;
    getElapsedTime(): number;
    restart(): void;
}

export class Music {
    loop: boolean;
    playingOffset: number;
    pitch: number;
    volume: number;
    position: Point3D;
    relativeToListener: boolean;
    minDistance: number;
    attenuation: number;

    constructor(filename: string);
    destroy(): void;
    getDuration(): number;
    play(): void;
    pause(): void;
    stop(): void;
    getChannelCount(): number;
    getSampleRate(): number;
    getStatus(): number;

    static createFromMemory(buffer: Buffer): Music;
}