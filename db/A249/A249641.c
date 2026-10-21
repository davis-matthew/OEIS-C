/*
 * A249641  Number of strings of length n over an 8-letter alphabet that begin
 *          with a nontrivial palindrome.
 * https://oeis.org/A249641
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, walk
 * Formula:   a(0)=0; a(1)=0; a(n+1)=8*a(n)+8^ceil((n+1)/2)-a(ceil((n+1)/2))
 *            for n>=0
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Use the provided integer types seq_t and idx_t. */
#include <stdlib.h>

/* Compute the nth term of OEIS sequence A249641. */
seq_t a(idx_t n)
{
    if (n < 2)                /* a(0) and a(1) are zero */
        return 0;

    /* Static tables for memoisation. */
    static seq_t *seq_arr  = NULL;   /* sequence values a[i]          */
    static size_t seq_len = 0;       /* number of computed entries    */
    static size_t seq_cap = 0;       /* current allocation capacity  */

    static seq_t *pow8_arr = NULL;   /* powers of 8, pow8_arr[k]=8^k */
    static size_t pow_len = 0;       /* number of computed powers    */
    static size_t pow_cap = 0;       /* current allocation capacity  */

    /* Initialise tables on first use. */
    if (seq_len == 0) {
        /* Allocate sequence table with capacity 2 */
        seq_cap = 2;
        seq_arr = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq_arr[0] = 0;
        seq_arr[1] = 0;
        seq_len = 2;

        /* Allocate powers-of-8 table with capacity 2 */
        pow_cap = 2;
        pow8_arr = (seq_t *)malloc(pow_cap * sizeof(seq_t));
        pow8_arr[0] = 1;         /* 8^0 */
        pow_len = 1;             /* only index 0 computed so far */
    }

    /* Compute any missing terms up to n. */
    if (seq_len <= (size_t)n) {
        /* Ensure the sequence table is large enough. */
        if (seq_cap <= (size_t)n) {
            size_t new_cap = seq_cap ? seq_cap : 2;
            while (new_cap <= (size_t)n)
                new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
            seq_arr = tmp;
            seq_cap = new_cap;
        }

        for (size_t i = seq_len; i <= (size_t)n; ++i) {
            /* k = ceil(i/2) = (i+1)/2 */
            size_t k = (i + 1) >> 1;

            /* Ensure powers of 8 up to 8^k are available. */
            if (pow_cap <= k) {
                size_t new_cap = pow_cap ? pow_cap : 2;
                while (new_cap <= k)
                    new_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(pow8_arr, new_cap * sizeof(seq_t));
                pow8_arr = tmp;
                pow_cap = new_cap;
            }
            /* Compute any missing powers. */
            for (size_t t = pow_len; t <= k; ++t)
                pow8_arr[t] = pow8_arr[t - 1] * (seq_t)8;
            pow_len = k + 1;

            /* a[i] = 8*a[i-1] + 8^k - a[k] */
            seq_t val = seq_arr[i - 1] * (seq_t)8;
            val += pow8_arr[k];
            val -= seq_arr[k];
            seq_arr[i] = val;
        }
        seq_len = (size_t)n + 1;
    }

    return seq_arr[(size_t)n];
}
