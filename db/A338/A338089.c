/*
 * A338089  Minimal number of moves for the cyclic variant of Hanoi's tower
 *          for 4 pegs and n disks, with the final peg three steps away.
 * https://oeis.org/A338089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) – 2*a(n-5) (n > 9) Initial
 *            values: a(1)=3, a(2)=10, a(3)=21, a(4)=40, a(5)=75, a(6)=134,
 *            a(7)=233, a(8)=400, a(9)=683 (OEIS A338089) The implementation
 *            memoises the sequence. It uses a static array resized as
 *            necessary; the array stores values for indices 0…n (inde...
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
    
seq_t a(idx_t n)
{
    /* undefined for n < 1 – return 0 as a safe default */
    if (n < 1) return (seq_t)0;
    
    /* minimal capacity: at least 10 to store the nine initial terms */
    size_t needed_cap = (size_t)n + 1;   /* indices 0…n inclusive */
    if (needed_cap < 10) needed_cap = 10;
    
    /* static storage for the memoised sequence */
    static seq_t *cache      = NULL;
    static size_t cap        = 0;     /* current allocated size (number of slots) */
    static size_t len        = 0;     /* largest index that has a computed value */
    
    /* auxiliary table of initial values */
    static const seq_t init_vals[10] = {
        0ULL,          /* dummy for index 0 */
        3ULL,   10ULL, 21ULL, 40ULL, 75ULL,
        134ULL, 233ULL, 400ULL, 683ULL
    };
    
    /* Resize array if required */
    if (cap < needed_cap) {
        size_t new_cap = needed_cap;
        seq_t *tmp;
#ifdef __cplusplus
        /* C++: allocate with new[], then copy existing entries */
        tmp = new seq_t[new_cap];
        if (cache) {
            for (size_t i = 0; i < cap; ++i) tmp[i] = cache[i];
            delete[] cache;
        }
#else
        /* C: use realloc.  The raw memory is fine for unsigned integer
           types or trivially default‑constructible classes. */
        tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
#endif
        cache = tmp;
        cap   = new_cap;
        /* Initialise the nine initial terms if they are not yet set */
        if (len < 9) {
            size_t upto = (cap - 1 < 9) ? (cap - 1) : 9;
            for (size_t i = len + 1; i <= upto; ++i) {
                cache[i] = init_vals[i];
            }
            len = upto;
        }
    }
    
    /* If the requested term is already computed, return it immediately */
    if ((size_t)n <= len) return cache[n];
    
    /* Compute successive terms up to n using the recurrence */
    for (idx_t i = (idx_t)(len + 1); i <= n; ++i) {
        /* The recurrence is only valid for i ≥ 10, but the loop
           never reaches i < 10 because len is at least 9 after
           initialisation above. */
        seq_t term = cache[i - 1] + cache[i - 2] + cache[i - 3];
        term -= cache[i - 5] + cache[i - 5];   /* subtract 2*a(i-5) */
        cache[i] = term;
    }
    len = (size_t)n;
    
    return cache[n];
}
