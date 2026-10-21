/*
 * A259714  a(n) = Sum_{k=1..n-1}((k mod 5)*a(n-k)), a(1) = 1.
 * https://oeis.org/A259714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 49, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (OEIS A259714):                 */
/*   a(1) = 1 ; for n > 1,                */
/*   a(n) = Σ_{k=1}^{n-1} ((k mod 5) * a(n-k))             */
/*   but for n > 6 a(n) satisfies the linear recurrence  */
/*   a(n) = a(n-1) + 2*a(n-2) + 3*a(n-3) + 4*a(n-4) + a(n-5)   */

#include <stdlib.h>

#ifdef __cplusplus
#include <new>
#endif

/* static memoisation buffer ------------------------------------------------ */
static seq_t *memo = NULL;      /* 1‑based: memo[n] contains a(n)         */
static idx_t memo_last = 0;     /* largest n for which memo[n] is valid  */
static size_t memo_cap  = 0;    /* allocated size (>= memo_last+1)      */

/* Ensure that memo has capacity for indices 0..n      */
static void ensure_capacity(idx_t n)
{
    if ((size_t)(n + 1) <= memo_cap)
        return;                 /* already sufficient                 */

    size_t new_cap = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* memory failure; terminate the program */
        exit(EXIT_FAILURE);
    }
    memo = tmp;
    memo_cap = new_cap;

#ifdef __cplusplus
    /* Construct the newly allocated elements (only needed for non‑trivial seq_t) */
    for (size_t i = memo_cap - (new_cap - memo_cap); i < new_cap; ++i) {
        new(&memo[i]) seq_t();  /* placement new */
    }
#endif
}

/* Compute a(i) for all i up to n, storing the results in memo               */
static void compute_up_to(idx_t n)
{
    if (memo_last >= n)
        return;                 /* already computed                     */

    ensure_capacity(n);

    for (idx_t i = memo_last + 1; i <= n; ++i) {
        seq_t value;
        if (i == 1) {
            value = 1;
        } else if (i <= 6) {      /* use the original sum definition for 1 ≤ n ≤ 6 */
            seq_t sum = 0;
            for (idx_t k = 1; k < i; ++k) {
                seq_t coeff = (seq_t)(k % 5);
                sum += coeff * memo[i - k];
            }
            value = sum;
        } else {                  /* linear recurrence of order 5 for n > 6     */
            value = memo[i - 1]
                    + (seq_t)2 * memo[i - 2]
                    + (seq_t)3 * memo[i - 3]
                    + (seq_t)4 * memo[i - 4]
                    + memo[i - 5];
        }
        memo[i] = value;
    }
    memo_last = n;
}

/*--- Public sequence function ------------------------------------------------*/
seq_t a(idx_t n)
{
    /* The function is defined for n >= 1; behaviour for n == 0 is undefined.  */
    compute_up_to(n);
    return memo[n];
}
