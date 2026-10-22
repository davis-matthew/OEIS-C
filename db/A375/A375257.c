/*
 * A375257  Numbers whose sum of base-2 digits is 1 more than their sum of
 *          base-3 digits.
 * https://oeis.org/A375257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth integer whose sum of binary digits * is one more
 *            than its sum of base‑3 digits
 * Range:     u8 n <= 29, u16 n <= 5409, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Count 1‑bits in a value of type seq_t (trivially copyable). */
static int popcount_seq(seq_t x)
{
    int cnt = 0;
    while (x)
    {
        x &= x - 1;
        ++cnt;
    }
    return cnt;
}

/* Return the nth term of A375257. Sequence is memoised locally. */
seq_t a(idx_t n)
{
    /* Static storage for memoised values.  All variables are
     * shared between calls so that repeated queries remain fast. */
    static seq_t *seq    = NULL;      /* pointer to cached terms */
    static size_t len    = 0;         /* number of terms stored */
    static size_t cap    = 0;         /* current capacity of the cache */
    static seq_t cand    = 0;         /* last integer examined */

    if (n == 0)
        return 0;                     /* undefined for offset 1, guard */

    /* Extend the cache until it contains at least n terms. */
    while (len < n)
    {
        /* Advance to next integer to test. */
        ++cand;

        /* Compute sum of binary digits. */
        int ones = popcount_seq(cand);

        /* Compute sum of base‑3 digits. */
        int sum3 = 0;
        seq_t tmp = cand;
        while (tmp)
        {
            sum3 += (int)(tmp % 3);
            tmp /= 3;
        }

        /* If the condition holds, store the number. */
        if (ones == sum3 + 1)
        {
            if (len == cap)
            {
                size_t new_cap = (cap == 0) ? 8 : cap * 2;
                seq_t *new_ptr = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                if (!new_ptr) abort();     /* allocation failure */
                seq = new_ptr;
                cap = new_cap;
            }
            seq[len++] = cand;
        }
    }

    return seq[n - 1];
}
