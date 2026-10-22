/*
 * A376184  Lexicographically earliest sequence of positive integers a(1),
 *          a(2), a(3), ... such that for any n > 0, S(n) = Sum_{k = 1..n}
 *          b(k)/a(k) < 1, where {b(k)} is the sequence b(1)=5/4, b(2*i)=3/2,
 *          b(2*i+1)=6/5 (i>0).
 * https://oeis.org/A376184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   lexicographically earliest a(n) such that S(n) = Σ_{k=1..n}
 *            b(k)/a(k) < 1, where b(1)=5/4, b(2k)=3/2, b(2k+1)=6/5 for k>0.
 *            The sequence is computed greedily: a(n) = ⌊ (b(n) * q_{n-1}) /
 *            (b_den * (q_{n-1} − p_{n-1})) ⌋ + 1 with p_{n-1}/q_{n-1} =
 *            S(n-1) reduced. P, Q are kept reduced
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* realloc */
#include <stdint.h>   /* for uint64_t if needed */

/* The types seq_t and idx_t are supplied by the build environment. */

/* Euclidean GCD for seq_t (supports % and ==); it works for any numeric
   type that supports the usual arithmetic operators. */
static inline seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Cached data for the sequence.  a_arr[1..] contains a(n);  stored_max
   is the largest n that has been computed.  p_arr and q_arr hold the
   numerator and denominator of S(n) in reduced form. */
static idx_t    stored_max = 0;      /* largest n processed so far      */
static size_t  capacity   = 0;      /* allocated array size            */
static seq_t *a_arr      = NULL;    /* a[1..]                          */
static seq_t *p_arr      = NULL;    /* S numerator  (p_arr[0] = 0)     */
static seq_t *q_arr      = NULL;    /* S denominator (q_arr[0] = 1) */

/* Ensure the arrays can hold indices up to 'need' (inclusive). */
static void ensure_capacity(idx_t need)
{
    if ((idx_t)capacity >= need + 1) return;   /* already enough */

    size_t new_cap = capacity ? capacity << 1 : 16;
    while (new_cap < (size_t)(need + 1)) new_cap <<= 1;

    a_arr = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
    p_arr = (seq_t *)realloc(p_arr, new_cap * sizeof(seq_t));
    q_arr = (seq_t *)realloc(q_arr, new_cap * sizeof(seq_t));

    /* initialise new slots to zero (p_arr[0] and q_arr[0] are set separately) */
    for (size_t i = capacity; i < new_cap; ++i) {
        a_arr[i] = (seq_t)0;
        p_arr[i] = (seq_t)0;
        q_arr[i] = (seq_t)0;
    }
    capacity = new_cap;
}

/* Main routine requested by the judge. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1 */

    /* initialise S(0) = 0/1 only once */
    if (stored_max == 0) {
        ensure_capacity(1);
        a_arr[0] = (seq_t)0;
        p_arr[0] = (seq_t)0;
        q_arr[0] = (seq_t)1;
    }

    /* extend cache up to n if needed */
    if (n > stored_max) {
        ensure_capacity(n);
        for (idx_t k = stored_max + 1; k <= n; ++k) {
            /* b(k): numerator and denominator */
            seq_t b_num, b_den;
            if (k == 1) {
                b_num = (seq_t)5;
                b_den = (seq_t)4;
            } else if ((k & 1) == 0) {          /* even k  */
                b_num = (seq_t)3;
                b_den = (seq_t)2;
            } else {                           /* odd k > 1 */
                b_num = (seq_t)6;
                b_den = (seq_t)5;
            }

            /* previous sum S(k-1) = p/q */
            seq_t p_prev = p_arr[k - 1];
            seq_t q_prev = q_arr[k - 1];

            /* compute a(k) */
            seq_t diff   = q_prev - p_prev;                 /* q-p > 0       */
            seq_t A_num  = b_num * q_prev;
            seq_t A_den  = b_den * diff;
            seq_t a_k    = (A_num / A_den) + (seq_t)1;       /* floor + 1     */

            a_arr[k] = a_k;

            /* update S(k) = S(k-1) + b(k)/a_k */
            seq_t T_num   = b_num;
            seq_t T_den   = b_den * a_k;

            seq_t g1      = gcd_seq(q_prev, T_den);
            seq_t mul1    = T_den / g1;      /* to multiply p_prev   */
            seq_t mul2    = q_prev / g1;     /* to multiply T_num    */

            seq_t numer_new = p_prev * mul1 + T_num * mul2;
            seq_t denom_new = q_prev * mul1;         /* lcm of denominators */

            seq_t g2      = gcd_seq(numer_new, denom_new);
            p_arr[k] = numer_new / g2;
            q_arr[k] = denom_new / g2;
        }
        stored_max = n;
    }
    return a_arr[n];
}
