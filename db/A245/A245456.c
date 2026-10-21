/*
 * A245456  Index of least Fibonacci number congruent to n mod 100.
 * https://oeis.org/A245456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) : index of the smallest Fibonacci number congruent to n (mod 100) */

#include <stdbool.h>   /* for bool, true, false */

seq_t a(idx_t n)
{
    /* static initialization of the residue-to-index table */
    static bool initialized = false;
    static const idx_t SENTINEL = (idx_t)-1;
    static idx_t map[100];

    if (!initialized) {
        /* initialise table entries to a sentinel value */
        for (int r = 0; r < 100; ++r)
            map[r] = SENTINEL;

        /* Fibonacci: F(0)=0, F(1)=1 */
        idx_t prev = 0, curr = 1;
        idx_t i = 1;                     /* current index (curr) */
        map[0] = 0;                      /* F(0) */
        map[1] = 1;                      /* F(1) */
        uint8_t seen = 2;                 /* residues discovered */

        while (seen < 100) {
            idx_t next = (prev + curr) % (idx_t)100;   /* F(i+1) mod 100 */
            ++i;                                     /* i now indexes F(i+1) */
            prev = curr;
            curr = next;
            if (map[next] == SENTINEL) {
                map[next] = i;
                ++seen;
            }
        }
        initialized = true;
    }

    /* n may be any non‑negative integer; use its residue modulo 100 */
    idx_t residue = n % (idx_t)100;
    return (seq_t)map[residue];
}
