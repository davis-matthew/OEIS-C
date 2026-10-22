/*
 * A400174  Number of steps starting from 2*n-1 until the first repeated value
 *          under the map k -> (5*k-1)/2^v_2(5*k-1) if k == 1 (mod 4) and k ->
 *          (5*k+1)/2^v_2(5*k+1) if k == 3 (mod 4) where v_2(k) = A007814(k);
 *          or -1 if no value ever repeats.
 * https://oeis.org/A400174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, new
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A400174: for each n ≥ 1 compute the smallest t > 0
   such that the t‑th iterate of the map
        f(k)= (5k-1)/2^v2(5k-1)   if k≡1 (mod 4)
        f(k)= (5k+1)/2^v2(5k+1)   if k≡3 (mod 4)
   equals a value already seen in the orbit starting at 2*n-1.  The value
   returned is that t.  The implementation uses Floyd's cycle finding
   algorithm and memoises results in a static array so that calls in any
   order are fast.  */
#include <stdlib.h>

/* Count the exponent of 2 dividing x (x > 0).  Works for any arithmetic
   seq_t type since shift and & are defined.  */
static unsigned count_trailing_zeros(seq_t x)
{
    unsigned c = 0;
    while ((x & 1) == 0) {
        x >>= 1;
        ++c;
    }
    return c;
}

/* One step of the map described above. */
static seq_t next_val(seq_t k)
{
    seq_t num;
    unsigned v;

    if ((k & 3) == 1) {                 /* k ≡ 1 (mod 4) */
        num = (seq_t)5 * k - 1;
    } else {                            /* k ≡ 3 (mod 4) */
        num = (seq_t)5 * k + 1;
    }
    v = count_trailing_zeros(num);
    return num >> v;
}

/* Main function requested by the build system.  */
seq_t a(idx_t n)
{
    /* Memoisation array: memo[i] holds a(i+1); 0 means not yet computed. */
    static seq_t *memo = NULL;
    static size_t memo_cap = 0;

    if (n < 1) return (seq_t)0;   /* should not happen for the OEIS offset */

    /* Ensure memo array is large enough. */
    if ((size_t)n > memo_cap) {
        size_t new_cap = memo_cap ? memo_cap << 1 : 64;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;     /* out of memory */
        /* Initialise newly allocated slots to 0 (uncomputed). */
        for (size_t i = memo_cap; i < new_cap; ++i) tmp[i] = (seq_t)0;
        memo = tmp;
        memo_cap = new_cap;
    }

    /* Return cached value if available. */
    if (memo[n - 1] != (seq_t)0) return memo[n - 1];

    /* Start of the orbit. */
    seq_t x0 = ((seq_t)n) * 2 - 1;    /* 2*n-1 */

    /* Floyd's cycle finding algorithm.  */
    seq_t tortoise = next_val(x0);
    seq_t hare = next_val(tortoise);

    while (tortoise != hare) {
        tortoise = next_val(tortoise);
        hare = next_val(next_val(hare));
    }

    /* Find mu (first index of cycle). */
    size_t mu = 0;
    tortoise = x0;
    while (tortoise != hare) {
        tortoise = next_val(tortoise);
        hare = next_val(hare);
        ++mu;
    }

    /* Find lambda (cycle length). */
    size_t lambda = 1;
    hare = next_val(tortoise);
    while (tortoise != hare) {
        hare = next_val(hare);
        ++lambda;
    }

    size_t answer = mu + lambda;
    memo[n - 1] = (seq_t)answer;
    return (seq_t)answer;
}
