/*
 * A322493  Start of first occurrence of n consecutive odd squarefree
 *          composite numbers.
 * https://oeis.org/A322493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) is found by brute‑force search for the first odd integer
 *            that starts a run of n consecutive odd squarefree composite
 *            numbers
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   is_prime_small, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_small.h"
#include "../utils/is_squarefree.h"

/* static cache: a[1]…a[8] are stored; value 0 means not yet computed.
   We allow up to index 9 for safety. */
static seq_t cache[10] = {0};

static inline bool composite_squarefree(seq_t x)
{
    /* x must be > 1, composite and squarefree */
    if (x <= 3) return false;
    if (is_prime_small(x)) return false;
    if (!is_squarefree(x)) return false;
    return true;
}

seq_t a(idx_t n)
{
    /* for n > 8 the sequence has no entries */
    if (n > 8) return 0;

    size_t idx = (size_t)n;
    if (cache[idx] != 0) return cache[idx];

    seq_t start = 9; /* first odd > 7 */
    for (;;)
    {
        bool ok = true;
        for (idx_t k = 0; k < n; ++k)
        {
            seq_t val = start + 2*k;
            if (!composite_squarefree(val))
            {
                ok = false;
                break;
            }
        }
        if (ok)
        {
            cache[idx] = start;
            return start;
        }
        start += 2;
    }
}
