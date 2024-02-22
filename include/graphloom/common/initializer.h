#ifndef GRAPHLOOM_COMMON_INITIALIZER_
#define GRAPHLOOM_COMMON_INITIALIZER_

namespace graphloom
{
    /**
     * Initalizer does nothing but allow 
     * code to run statically by using its constructor.
     * 
     * Example:
     * 
     * Initalizer Baz()
     * {
     *      // arbitrary code
     *      return Initalizer();
     * }
     * 
     * // Run any code before main
     * Initalizer foo = Baz();
     * 
     * int main()
     * {
     *      
     * }
    */
    class Initializer
    {
    public:
        Initializer() = default;

        Initializer(const Initializer&) {};
    };


    
}

#endif