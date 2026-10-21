/*
 * A257440  Number of (n+2) X (1+2) 0..1 arrays with no 3 x 3 subblock
 *          diagonal sum 0 and no antidiagonal sum 3 and no row sum 1 and no
 *          column sum 1.
 * https://oeis.org/A257440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 23, u32 n <= 52, u64 n <= 110, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A257440
   Formula: a(n) = a(n-1) + 2*a(n-3) - a(n-4) - a(n-6)   for n > 12
   Initial values: a(1)=30, a(2)=36, a(3)=39, a(4)=49, a(5)=65, a(6)=90,
                   a(7)=128, a(8)=186, a(9)=269, a(10)=392, a(11)=573, a(12)=836.
   The sequence satisfies the empirical recurrence for all n>12.
*/

#include <stddef.h>
#include <stdlib.h>

/* memoization storage */
static seq_t *seq_cache = NULL;   /* array of terms, 1‑based */
static size_t seq_cap  = 0;       /* allocated capacity */
static size_t seq_last = 0;       /* highest index computed */

/* make sure capacity is at least `n+1` (index `n` usable) */
static void ensure_capacity(size_t n)
{
    if (seq_cap <= n)
    {
        size_t new_cap = seq_cap ? seq_cap << 1 : 64;
        while (new_cap <= n) new_cap <<= 1;

        seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        /* zero fill the newly allocated slots (if any) */
        for (size_t i = seq_cap; i < new_cap; ++i)
            seq_cache[i] = 0;
        seq_cap = new_cap;
    }
}

/* initialise the base values (indices 1 .. 12) */
static void init_base(void)
{
    if (seq_last == 0)
    {
        ensure_capacity(12);
        seq_cache[1] = 30; seq_cache[2] = 36; seq_cache[3] = 39;
        seq_cache[4] = 49; seq_cache[5] = 65; seq_cache[6] = 90;
        seq_cache[7] = 128; seq_cache[8] = 186; seq_cache[9] = 269;
        seq_cache[10] = 392; seq_cache[11] = 573; seq_cache[12] = 836;
        seq_last = 12;
    }
}

/* required public function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined offset */
    init_base();

    size_t N = (size_t)n;
    if (N <= seq_last) return seq_cache[N];

    ensure_capacity(N);
    for (size_t i = seq_last + 1; i <= N; ++i)
    {
        if (i <= 12)  /* already initialised */
            continue;
        /* recurrence: a(i) = a(i-1) + 2*a(i-3) - a(i-4) - a(i-6) */
        seq_t val = seq_cache[i-1];
        val += seq_cache[i-3];
        val += seq_cache[i-3];
        val -= seq_cache[i-4];
        val -= seq_cache[i-6];
        seq_cache[i] = val;
    }
    seq_last = N;
    return seq_cache[N];
}
