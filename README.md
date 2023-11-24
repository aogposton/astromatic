# astromatic
Tools to save you a few seconds when using astro.

To use this tool,
1. download the repo
2. compile the source on your machine
3. move the executable to the root directory of our astro folder
4. start using commands


astromatic assumes your written content is located in the folder "./src/content.
If that is not the folder where your content is stored, create a file called astromatic.json and place it in your root directory beside astromatic.

The astromatic json file should look like this:

```
{
    "contentFolder":"./src/content",
    "author": "authors name"
}
```

If you have a key in the astromatic.json file that matches a key of your generated document it will be used as the default value.

### Commands
**./astromatic generate**
generates a file in your content folder
