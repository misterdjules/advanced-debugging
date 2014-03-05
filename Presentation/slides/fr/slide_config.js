var SLIDE_CONFIG = {
  // Slide settings
  settings: {
    title: 'Débogage avancé avec Visual C++ 2013',
    subtitle: 'Outils & méthodes',
    eventInfo: {
      title: 'Atelier DDJV',
      date: '03/05/2014'
    },
    useBuilds: true, // Default: true. False will turn off slide animation builds.
    usePrettify: true, // Default: true
    enableSlideAreas: true, // Default: true. False turns off the click areas on either slide of the slides.
    enableTouch: true, // Default: true. If touch support should enabled. Note: the device must support touch.
    //analytics: 'UA-XXXXXXXX-1', // TODO: Using this breaks GA for some reason (probably requirejs). Update your tracking code in template.html instead.
    favIcon: 'images/DDJV/sherbrooke_university_logo_favicon.png',
    fonts: [
      'Open Sans:regular,semibold,italic,italicsemibold',
      'Source Code Pro'
    ],
    theme: ['ddjv-2014'], // Add your own custom themes or styles in /theme/css. Leave off the .css extension.
  },

  // Author information
  presenters: [{
    name: 'Julien Gilli',
    company: 'Développeur indépendant',
    //gplus: 'http://plus.google.com/1234567890',
    twitter: '@misterdjules',
    www: 'http://www.juliengilli.com',
    github: 'http://github.com/misterdjules'
  }/*, {
    name: 'Second Name',
    company: 'Job Title, Google',
    gplus: 'http://plus.google.com/1234567890',
    twitter: '@yourhandle',
    www: 'http://www.you.com',
    github: 'http://github.com/you'
  }*/]
};

