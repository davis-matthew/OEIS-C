/*
 * A283568  Number of n X 4 0..1 arrays with no 1 equal to more than one of
 *          its horizontal, diagonal and antidiagonal neighbors, with the
 *          exception of exactly one element.
 * https://oeis.org/A283568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*a(n-1) -16*a(n-2) -148*a(n-3) +122*a(n-4) +8*a(n-5)
 *            -1218*a(n-6) +1236*a(n-7) +1851*a(n-8)
 *            -3776*a(n-9)+3314*a(n-10)+5408*a(n-11)
 *            -7569*a(n-12)-2532*a(n-13)+2620*a(n-14) +320*a(n-15)-256*a(n-16)
 *            Initial terms (a(1) … a(16)): 2, 72, 674, 6812, 60802, 528436,
 *            4441052, 36589848, 296555892,...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The first 16 terms are known. */
    static const seq_t init[16] = {
        2ULL,
        72ULL,
        674ULL,
        6812ULL,
        60802ULL,
        528436ULL,
        4441052ULL,
        36589848ULL,
        296555892ULL,
        2373574616ULL,
        18804085974ULL,
        147722885964ULL,
        1152326125736ULL,
        8934988081564ULL,
        68923216977218ULL,
        529275667161388ULL
    };

    /* Circular buffer holding the most recent 16 values. */
    static seq_t buf[16];
    static idx_t computed_max = 0;   /* highest n that has been computed */

    /* Return known term immediately for n <= 16. */
    if (n <= 16)
        return init[n - 1];

    /* Initialise the buffer on first use. */
    if (computed_max == 0) {
        for (idx_t i = 0; i < 16; ++i)
            buf[i] = init[i];
        computed_max = 16;
    }

    /* Compute values up to n, if needed. */
    for (idx_t k = computed_max + 1; k <= n; ++k) {
        seq_t a1  = buf[(k - 2) % 16];/* a(k-1)  */
        seq_t a2  = buf[(k - 3) % 16];/* a(k-2)  */
        seq_t a3  = buf[(k - 4) % 16];/* a(k-3)  */
        seq_t a4  = buf[(k - 5) % 16];/* a(k-4)  */
        seq_t a5  = buf[(k - 6) % 16];/* a(k-5)  */
        seq_t a6  = buf[(k - 7) % 16];/* a(k-6)  */
        seq_t a7  = buf[(k - 8) % 16];/* a(k-7)  */
        seq_t a8  = buf[(k - 9) % 16];/* a(k-8)  */
        seq_t a9  = buf[(k -10) % 16];/* a(k-9)  */
        seq_t a10 = buf[(k -11) % 16];/* a(k-10) */
        seq_t a11 = buf[(k -12) % 16];/* a(k-11) */
        seq_t a12 = buf[(k -13) % 16];/* a(k-12) */
        seq_t a13 = buf[(k -14) % 16];/* a(k-13) */
        seq_t a14 = buf[(k -15) % 16];/* a(k-14) */
        seq_t a15 = buf[(k -16) % 16];/* a(k-15) */
        seq_t a16 = buf[(k -17) % 16];/* a(k-16) */

        seq_t val = 0;
        val += 12  * a1;
        val -= 16  * a2;
        val -= 148 * a3;
        val += 122 * a4;
        val += 8   * a5;
        val -= 1218* a6;
        val += 1236* a7;
        val += 1851* a8;
        val -= 3776* a9;
        val += 3314* a10;
        val += 5408* a11;
        val -= 7569* a12;
        val -= 2532* a13;
        val += 2620* a14;
        val += 320 * a15;
        val -= 256 * a16;

        buf[(k - 1) % 16] = val;   /* store a(k) */
    }

    computed_max = n;
    return buf[(n - 1) % 16];
}
