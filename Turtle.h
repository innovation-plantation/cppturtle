/**
 * This simple program implements turtle graphics in under 100 lines of C++ code
 * (excluding comments and blank lines)
 */

#ifndef CPPTURTLE_TURTLE_H
#define CPPTURTLE_TURTLE_H

#include <thread>
#include <mutex>
#include <complex>
#include <gtkmm.h>
#include <gtkmm/application.h>
#include <gtkmm/window.h>
#include <gtkmm/drawingarea.h>

/**
 * Simple Turtle Graphics implementation based on GTKMM
 * The only code required in turtle.cpp besides including this file is
 *
 * turtle::Turtle turtle::turtle;
 *
 * The main program can then use namespace turtle to call
 * lt(degrees) ;   // to  turn degrees left / right
 * rt(degrees) ;
 * fd(distance) ;   // to move forward / backward pixels distance
 * bk(distance) ;
 * pu() ;   // pen up / down
 * pd() ;
 * ht() ;   // hide / show turtle
 * st() ;
 */
namespace turtle {

    using namespace std;
    using namespace this_thread;
    using namespace chrono_literals;

    using namespace Gtk;

    typedef Cairo::RefPtr<Cairo::Context> Pad;
    typedef function<void(const Pad &)> Step;

    /**
     * The Turtle class implements the turtle graphics application & main window.
     * The graphics library has restrictions that only allow one application instance,
     * so only one instance of Turtle should be created.
     */
    extern class Turtle {
        Glib::RefPtr<Application> app = Application::create();
        vector<Step> steps; // Sequence of function to redraw the screen

        complex<double> pt;
        double theta = 0;
        bool marking = true;
        bool hiding = false;

        /**
         * The drawing area exists to supply its on_draw method to GTK++
         * The on_draw of this method renders the scene that represents
         * a field in which a turtle walks or a drawing pad on which the image is drawn
         */
        class Drawing : public DrawingArea {
            // DrawingArea can't be the same class as Turtle because
            // its constructor can't be called until after
            // Application::Create is called by Turtle's run method
            //
            Turtle &parent;
            bool on_draw(const Pad &pad) {
                pad->translate(100, 100);
                parent.pt = 0;
                parent.theta = 0;
                parent.marking = true;
                pad->move_to(0, 0);
                for (Step &step:parent.steps) step(pad);
                pad->stroke();
                if (parent.hiding) return true;
                pad->set_source_rgba(0, .25, 0, .25); // turtle color, position, pose
                pad->translate(parent.pt.real(), -parent.pt.imag());
                pad->rotate(-parent.theta);
                pad->save(); // body
                pad->scale(1.25, 1);
                pad->arc(0, 0, 5, -M_PI, M_PI);
                pad->restore();
                pad->fill();
                pad->arc(-4, -5, 1.5, -M_PI, M_PI); //  legs
                pad->arc(-4, 5, 1.5, -M_PI, M_PI);
                pad->arc(4, -5, 1.5, -M_PI, M_PI);
                pad->arc(4, 5, 1.5, -M_PI, M_PI);
                pad->fill();
                pad->arc(8, 0, 2, -M_PI, M_PI); // head
                pad->fill();
                return true;
            }
        public:
            /**
             * Construct the turtle graphics environment / turtle object that will draw the scene
             * @param parent
             */
            Drawing(Turtle *parent) : parent{*parent} {}
        };
        Drawing *drawing;
        thread background;
        /**
         * Note that calling app->run blocks until the program ends.
         * Therefore, it needs to run in its own thread so that the C++ program can
         * issue commands to control the turtle while this runs.
         */
        void run(mutex &sync) {
            Window win;
            Drawing drawing(this);
            this->drawing = &drawing;
            drawing.show();
            win.add(drawing);
            sync.unlock();
            app->run(win);
            this->drawing = nullptr;
        }
        /**
         * This adds a bit of code to the steps used for redrawing the screen.
         * @param step
         */
        void addStep(const Step &step) {
            sleep_for(.5s);
            steps.push_back(step);
            drawing->queue_draw();
            yield();
        }
    public:
        /**
         * Get the turtle graphics set up and running in the background, then return to let C++ code call
         * functions that control the turtle.
         */
        Turtle() {
            mutex sync;
            sync.lock();
            background = thread([this,&sync] { this->run(sync);});
            sync.lock();
        }
        /**
         * Code executed to render a move forward while updating the state accordingly
         * @param distance
         */
        void forward(double distance) {
            if (marking) {
                addStep([distance, this](const Pad &pad) {
                    pt += polar(distance, theta);
                    pad->line_to(pt.real(), -pt.imag());
                });
            } else {
                addStep([distance, this](const Pad &pad) {
                    pt += polar(distance, theta);
                    pad->move_to(pt.real(), -pt.imag());
                });
            }
        }
        /**
         * @param distance
         * Code to update the state for the new heading.
         */
        void turn(double degrees) {addStep([degrees, this](const Pad &g) {theta += degrees * M_PI / 180;});}
        /**
         * Moving while the marking state is true leaves marks wherever the turtle travels
         * @param state
         */
        void setMarking(bool state) { marking = state; }
        /**
         * If the hiding state is true, the turtle will be not be drawn.
         * @param state
         */
        void setHiding(bool state) { hiding = state; drawing->queue_draw(); }
    } turtle;
    /**
     * Turn the turtle right by the specified angle
     * @param degrees
     */
    inline void rt(double degrees) { turtle.turn(-degrees); }
    /**
     * Turn the turtle left  by the specified angle
     * @param degrees
     */
    inline void lt(double degrees) { turtle.turn(degrees); }
    /**
     * Move the turtle backwards by the number of pixels
     * @param distance
     */
    inline void bk(double distance) { turtle.forward(-distance); }
    /**
     * Move the turtle forward by the number of pixels
     * @param distance
     */
    inline void fd(double distance) { turtle.forward(distance); }
    /**
     * The turtle lifts the pen down off of the paper so that any future moves will not leave marks on the paper
     */
    inline void pu() { turtle.setMarking(false); }
    /**
     * The turtle puts the pen down onto the paper so that any future moves will leave marks on the paper
     */
    inline void pd() { turtle.setMarking(true); }
    /**
     * Hide the turtle so that only the drawing is visible
     */
    inline void ht() { turtle.setHiding(true); }
    /**
     * Show the turtle so that not only the drawing is visible, but also the turtle will be visible.
     */
    inline void st() { turtle.setHiding(false); }
};

#endif //CPPTURTLE_TURTLE_H
