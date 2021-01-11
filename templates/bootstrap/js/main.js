; (function ($) {
    "use strict";

    $(document).ready(function () {

        /**-------------------------------------------
         *  Navbar fix
         * -----------------------------------------*/
        $(document).on('click', '.navbar-area .navbar-nav li.menu-item-has-children>a', function (e) {
            e.preventDefault();
        })
       
        /*-------------------------------------
            menu
        -------------------------------------*/
        $('.navbar-area .menu').on('click', function() {
            $(this).toggleClass('open');
            $('.navbar-area .navbar-collapse').toggleClass('sopen');
        });
    
        // mobile menu
        if ($(window).width() < 992) {
            $(".in-mobile").clone().appendTo(".sidebar-inner");
            $(".in-mobile ul li.menu-item-has-children").append('<i class="fas fa-chevron-right"></i>');
            $('<i class="fas fa-chevron-right"></i>').insertAfter("");

            $(".menu-item-has-children a").on('click', function(e) {
                // e.preventDefault();

                $(this).siblings('.sub-menu').animate({
                    height: "toggle"
                }, 300);
            });
        }

        var menutoggle = $('.menu-toggle');
        var mainmenu = $('.navbar-nav');
        
        menutoggle.on('click', function() {
            if (menutoggle.hasClass('is-active')) {
                mainmenu.removeClass('menu-open');
            } else {
                mainmenu.addClass('menu-open');
            }
        });

        /*--------------------------------------------------
            select onput
        ---------------------------------------------------*/
        if ($('.single-select').length){
            $('.single-select').niceSelect();
        }

        /* --------------------------------------------------
            city 
        ---------------------------------------------------- */
        var $cityFilterArea = $('.feedback-filter-area');
        /*Grid*/
        $cityFilterArea.each(function(){
            var $this = $(this),
            $cityFilterItem = '.dmne-fdb-item';
            $this.imagesLoaded( function() {
                $this.isotope({
                    itemSelector: $cityFilterItem,
                    percentPosition: true,
                    masonry: {
                        columnWidth: '.fdb-sizer',
                    }
                });
            });
        });

        /* -----------------------------------------------------
            Variables
        ----------------------------------------------------- */
        var leftArrow = '<i class="fa fa-angle-left"></i>';
        var rightArrow = '<i class="fa fa-angle-right"></i>';

        /*------------------------------------------------
            exp-slider
        ------------------------------------------------*/
        $('.exp-slider').owlCarousel({
            loop: true,
            margin: 10,
            nav: false,
            dots: true,
            smartSpeed:1500,
            items: 1,
        });

        /*------------------------------------------------
            Courses-slider
        ------------------------------------------------*/
        $('.courses-slider').owlCarousel({
            loop: true,
            margin: 30,
            nav: false,
            dots: true,
            smartSpeed:1500,
            responsive: {
                0: {
                    items: 1,
                },
                768: {
                    items: 2
                },
                992: {
                    items: 3
                }
            }
        });

        /*------------------------------------------------
            feedback-slider
        ------------------------------------------------*/
        $('.feedback-slider').owlCarousel({
            loop: true,
            margin: 30,
            nav: false,
            dots: true,
            smartSpeed:1500,
            navText: [ leftArrow, rightArrow],
            responsive: {
                0: {
                    items: 1,
                    margin: 10,
                },
                768: {
                    items: 2
                },
                992: {
                    items: 3
                },
                1600: {
                    items: 4,
                    margin: 30,
                },
                2000: {
                    items: 5,
                    margin: 30,
                },
            }
        });

        /*------------------------------------------------
            sponsor-slider
        ------------------------------------------------*/
        $('.client-slider').owlCarousel({
            loop: true,
            margin: 10,
            nav: false,
            dots: false,
            smartSpeed:1500,
            navText: [ leftArrow, rightArrow],
            responsive: {
                0: {
                    items: 2
                },
                576: {
                    items: 3
                },
                992: {
                    items: 5
                },
            }
        });

        /*------------------------------------------------
            sponsor-slider
        ------------------------------------------------*/
        $('.welcome-slider').owlCarousel({
            loop: true,
            margin: 30,
            nav: false,
            dots: true,
            smartSpeed:1500,
            navText: [ leftArrow, rightArrow],
            responsive: {
                0: {
                    items: 1
                },
                992: {
                    items: 2
                },
            }
        });

        /*------------------------------------
            Product Details Slider
        ------------------------------------*/
         /*------------------------------------
            Product Details Slider
        ------------------------------------*/
        var productDetailSlider = $('.testimonial-thumbnail-slider');
        var pThumbanilSlider = $('.testimonial-thumbnail-carousel');

        if (productDetailSlider.length) {
            productDetailSlider.slick({
                slidesToShow: 1,
                slidesToScroll: 1,
                arrows: false,
                dots: true,
                fade: true,
                asNavFor: '.testimonial-thumbnail-carousel'
              });
        }
        if (pThumbanilSlider.length) {
            pThumbanilSlider.slick({
                slidesToShow: 5,
                slidesToScroll: 1,
                asNavFor: '.testimonial-thumbnail-slider',
                dots: false,
                centerMode: false,
                focusOnSelect: true,
                arrows:false,
              });
        }

        /*--------------------------------------------------------
            video popup 
        --------------------------------------------------------*/
        $('.video-play-btn').magnificPopup({
            type: 'video',
            removalDelay: 260,
            mainClass: 'mfp-zoom-in',
        });

        /*-------------------------------------------------
           parallax
        --------------------------------------------------*/
        if ($.fn.jarallax) {
            $('.jarallax').jarallax({
                speed: 0.5
            });
        }

        /*--------------------------------------------
            Search Popup
        ---------------------------------------------*/
        var bodyOvrelay =  $('#body-overlay');
        var searchPopup = $('#td-search-popup');

        $(document).on('click','#body-overlay',function(e){
            e.preventDefault();
        bodyOvrelay.removeClass('active');
            searchPopup.removeClass('active');
        });
        $(document).on('click','.search',function(e){
            e.preventDefault();
            searchPopup.addClass('active');
        bodyOvrelay.addClass('active');
        });

        /*--------------------------------------------
            log-in Popup
        ---------------------------------------------*/
        var bodyOvrelay =  $('#body-overlay');
        var logInPopup = $('#td-log-in-popup');

        $(document).on('click','#body-overlay',function(e){
            e.preventDefault();
        bodyOvrelay.removeClass('active');
            logInPopup.removeClass('active');
        });
        $(document).on('click','.log-in',function(e){
            e.preventDefault();
            logInPopup.addClass('active');
        bodyOvrelay.addClass('active');
        });


        /**---------------------------------------
         *  Progress BAR
        * -------------------------------------*/
        jQuery(window).on('scroll', function() {
            var windowHeight = $(window).height();
            function kalProgress() {
               var progress = $('.progress-rate');
               var len = progress.length;
                for (var i = 0; i < len; i++) {
                   var progressId = '#' + progress[i].id;
                   var dataValue = $(progressId).attr('data-value');
                   $(progressId).css({'width':dataValue+'%'});
                }
            }
            var progressRateClass = $('#progress-running');
             if ((progressRateClass).length) {
                 var progressOffset = $("#progress-running").offset().top - windowHeight;
                 if ($(window).scrollTop() > progressOffset) {
                     kalProgress();
                }
            }
        });

        /*-------------------------------------------------
            wow js init
        --------------------------------------------------*/
        new WOW().init();

        /*------------------
           back to top
        ------------------*/
        $(document).on('click', '.back-to-top', function () {
            $("html,body").animate({
                scrollTop: 0
            }, 2000);
        });

    });

    $(window).on("scroll", function() {
        /*---------------------------------------
        sticky menu activation && Sticky Icon Bar
        -----------------------------------------*/

        var mainMenuTop = $(".navbar-area");
        if ($(window).scrollTop() >= 1) {
            mainMenuTop.addClass('navbar-area-fixed');
        }
        else {
            mainMenuTop.removeClass('navbar-area-fixed');
        }
        
        var ScrollTop = $('.back-to-top');
        if ($(window).scrollTop() > 1000) {
            ScrollTop.fadeIn(1000);
        } else {
            ScrollTop.fadeOut(1000);
        }
    });


    $(window).on('load', function () {

        /*-----------------
            preloader
        ------------------*/
        var preLoder = $("#preloader");
        preLoder.fadeOut(0);

        /*-----------------
            back to top
        ------------------*/
        var backtoTop = $('.back-to-top')
        backtoTop.fadeOut();

        /*---------------------
            Cancel Preloader
        ----------------------*/
        $(document).on('click', '.cancel-preloader a', function (e) {
            e.preventDefault();
            $("#preloader").fadeOut(2000);
        });

    });



})(jQuery);