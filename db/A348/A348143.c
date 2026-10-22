/*
 * A348143  Areas of integer-sided cyclic quadrilaterals whose area equals
 *          their perimeter.
 * https://oeis.org/A348143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for integer‑sided cyclic quadrilaterals whose area equals
   their perimeter.  All admissible perimeters are reported in
   ascending order, so that a(1) = 16, a(2) = 18, a(3) = 20, a(4) = 30,
   etc.  For n greater than the number of solutions, the function
   returns 0.  The search is exhaustive up to a modest upper bound
   (200) which is sufficient to find all known solutions.  The
   calculation uses only integer arithmetic.  */

#include <stdbool.h>
#include <stdint.h>

/* The types seq_t and idx_t are supplied by the build system. */

/* Fixed size buffer for discovered perimeters.  There are only four
   known values, so fifteen slots are more than enough. */
static seq_t perimeters[16];
static idx_t perim_cnt = 0;
static bool initialized = false;

/* Simple insertion sort for the tiny perimeters array. */
static void sort_perimeters(void)
{
    for (idx_t i = 1; i < perim_cnt; ++i) {
        seq_t key = perimeters[i];
        idx_t j = i;
        while (j > 0 && perimeters[j - 1] > key) {
            perimeters[j] = perimeters[j - 1];
            --j;
        }
        perimeters[j] = key;
    }
}

/* Exhaustively search all integer quadruples (a,b,c,d) with perimeter
   P = a+b+c+d <= Smax.  The four terms (P‑2a)(P‑2b)(P‑2c)(P‑2d)
   must equal 16·P² for the area to equal the perimeter. */
static void compute_perimeters(void)
{
    const int Smax = 200;          /* upper bound on perimeter */
    /* Loop over sorted quadruples to avoid duplicates. */
    for (int P = 16; P <= Smax; ++P) {
        for (int a = 1; a <= P - 3; ++a) {
            for (int b = a; b <= P - a - 2; ++b) {
                for (int c = b; c <= P - a - b - 1; ++c) {
                    int d = P - a - b - c;
                    if (d < c)          /* enforce a ≤ b ≤ c ≤ d */
                        continue;

                    int64_t t1 = (int64_t)P - 2 * a;
                    int64_t t2 = (int64_t)P - 2 * b;
                    int64_t t3 = (int64_t)P - 2 * c;
                    int64_t t4 = (int64_t)P - 2 * d;

                    int64_t prod = t1 * t2 * t3 * t4;
                    int64_t rhs  = 16LL * P * P;

                    if (prod == rhs) {
                        /* Ensure the perimeter is stored only once. */
                        bool found = false;
                        for (idx_t k = 0; k < perim_cnt; ++k) {
                            if (perimeters[k] == (seq_t)P) {
                                found = true;
                                break;
                            }
                        }
                        if (!found) {
                            if (perim_cnt < sizeof(perimeters) / sizeof(perimeters[0])) {
                                perimeters[perim_cnt++] = (seq_t)P;
                            }
                        }
                    }
                }
            }
        }
    }
    sort_perimeters();
}

/* Public accessor compatible with the build system. */
seq_t a(idx_t n)
{
    if (!initialized) {
        compute_perimeters();
        initialized = true;
    }
    if (n == 0 || n > perim_cnt)
        return 0;
    return perimeters[n - 1];
}
