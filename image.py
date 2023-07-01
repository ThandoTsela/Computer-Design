from PIL import Image

# Open the image
image = Image.open('superman.jpeg')

# Get the size of the image
width, height = image.size

#this weight
#print(width)

#this weight
#print(height)# Create an array to store the pixel values

print(" ")
pixel_array = []

# Loop through each pixel in the image
for y in range(height):
    for x in range(width):
        # Get the RGB values of the pixel
        pixel = image.getpixel((x, y))
        # Add the pixel to the array
        pixel_array.append(pixel)
final = []
#print the array
# Loop through the pixel array and print each pixel
for pixel in pixel_array:
        r,g,b = pixel[0],pixel[1],pixel[2]
        res = r << 16
        res += g << 8
        res += b
        final.append(res)
print(final)
        

# Convert each pixel to its hex code value and store it in a new array
#hex_array = [f"#{pixel[0]:02x}{pixel[1]:02x}{pixel[2]:02x}" for pixel in pixel_array]
#print(hex_array)

# Convert each hex code to its decimal value and store it in a new array
#decimal_array = [int(hex_code[1:], 16) for hex_code in hex_array]
#print(decimal_array)
# Define the size of the image
# width = 128
# height = 128

# # Create an empty image
# image = Image.new('RGB', (width, height))

# # Create an array of pixel values
# #pixel_array = [(255, 0, 0) for i in range(width * height)]

# # Put the pixels in the image
# image.putdata(pixel_array)

# # Show the image
# image.show()
