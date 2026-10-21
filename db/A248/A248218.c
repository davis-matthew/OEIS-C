/*
 * A248218  Period in residues modulo n in iteration of x^2 + 1 starting at 0.
 * https://oeis.org/A248218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 572, u16 n <= 9918, u32 n <= 9918, u64 n <= 9918,
 *            u128 n <= 9918, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = length of the eventual cycle of the map
   f(x) = (x^2 + 1) mod n starting from x = 0.
   The algorithm follows the sequence until a value repeats;
   the difference between the first and second occurrence of that
   value is the period.  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1; 0 is undefined */

    /* allocate an array to remember when each residue first appeared */
    size_t size = (size_t)n;
    int *seen = (int*)malloc(size * sizeof(int));
    if (!seen) return 0;           /* allocation failure → undefined result */

    for (size_t i = 0; i < size; ++i) seen[i] = -1;

    seq_t x = 0;                   /* current residue */
    idx_t step = 0;                /* number of iterations performed */

    while (1) {
        size_t idx = (size_t)x;    /* x is always < n, so fits in size_t */
        if (seen[idx] != -1) {     /* residue repeats → cycle found */
            int first = seen[idx];
            free(seen);
            return (seq_t)(step - first);
        }
        seen[idx] = (int)step;

        /* next = (x^2 + 1) mod n, computed safely in 128‑bit to avoid overflow */
        seq_t mod  = (seq_t)n;
        seq_t nx   = ((unsigned __int128)x * x + 1) % mod;
        x = nx;
        ++step;
    }
}
