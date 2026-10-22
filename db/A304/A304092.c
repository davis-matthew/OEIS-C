/*
 * A304092  Number of Lucas numbers (A000032: 2, 1, 3, 4, 7, 11, ...) dividing
 *          n.
 * https://oeis.org/A304092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of Lucas numbers (A000032) that divide n
 * Range:     u8 n <= 9851, u16 n <= 9851, u32 n <= 9851, u64 n <= 9851,
 *            u128 n <= 9851, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* External function definition as required */
seq_t a(idx_t n)
{
    /* Lazily build the list of Lucas numbers that fit in uint64_t.
       (The maximal Lucas number <= UINT64_MAX is L_92, about 7.5e18.) */
    static unsigned __int128 *lucas = NULL;   /* array of Lucas numbers */
    static size_t lucas_cnt = 0;              /* number of elements */

    if (!lucas) {
        /* Initial capacity: 50, will grow if needed */
        size_t cap = 50;
        lucas = (unsigned __int128 *)malloc(cap * sizeof(unsigned __int128));
        size_t idx = 0;

        unsigned __int128 L0 = 2;
        unsigned __int128 L1 = 1;
        lucas[idx++] = L0;
        lucas[idx++] = L1;

        while (1) {
            unsigned __int128 L_next = L0 + L1;
            if (L_next > (unsigned __int128)UINT64_MAX)
                break;

            if (idx >= cap) {           /* grow the array if needed */
                cap *= 2;
                lucas = (unsigned __int128 *)realloc(lucas,
                                                     cap * sizeof(unsigned __int128));
            }
            lucas[idx++] = L_next;
            L0 = L1;
            L1 = L_next;
        }
        lucas_cnt = idx;
    }

    /* Count Lucas numbers that divide n. */
    seq_t count = 0;
    uint64_t val_n = (uint64_t)n;

    for (size_t i = 0; i < lucas_cnt; ++i) {
        uint64_t l = (uint64_t)lucas[i];
        /* If l > val_n, val_n % l == val_n, which is not zero unless val_n == 0.
           So no early break is needed. */
        if (l != 0 && val_n % l == 0)
            ++count;
    }

    return count;
}
