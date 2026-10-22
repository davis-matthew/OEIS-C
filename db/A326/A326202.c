/*
 * A326202  Lexicographically earliest sequence such that for all i, j, a(i) =
 *          a(j) => f(i) = f(j), where f(n) = 0 if n > 2 and gcd(n,sigma(n)) =
 *          1, with f(n) = n for all other numbers.
 * https://oeis.org/A326202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Lexicographically earliest sequence such that a(i) = a(j) ⇒ f(i)
 *            = f(j), where f(n) = 0 if n > 2 and gcd(n,σ(n)) = 1, f(n) = n
 *            otherwise
 * Range:     u8 n <= 443, u16 n <= 9633, u32 n <= 9633, u64 n <= 9633,
 *            u128 n <= 9633, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Necessary types and headers are supplied by the build system. */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/gcd.h"          /* seq_t gcd(seq_t, seq_t); */

/* Maximum value that can appear on the right‑hand side of the sequence.
   For n up to 10 000 the values never exceed 50 000, so a fixed
   buffer is sufficient for both C and C++ builds. */
#define MAX_VAL (50000)

static seq_t a_arr[MAX_VAL + 1];         /* a(n) values */
static bool  used_val[MAX_VAL + 1];      /* has value v already been used? */
static seq_t value_f[MAX_VAL + 1];       /* f(v) stored for each value v */
static bool  init_done = false;
static idx_t cur_n    = 0;              /* largest n already computed */

/* Compute σ(n) = sum of positive divisors of n,
   using a simple O(√n) algorithm (fast enough for n ≤ ~10⁴). */
static seq_t sigma_of_num(idx_t n)
{
    seq_t sum = 0;
    /* integer square root of n */
    idx_t root = 1;
    while ((root + 1) * (root + 1) <= n) ++root;

    for (idx_t d = 1; d <= root; ++d)
    {
        if (n % d == 0)
        {
            sum += (seq_t)d;
            idx_t other = n / d;
            if (other != d) sum += (seq_t)other;
        }
    }
    return sum;
}

/* Compute f(n) according to the definition. */
static seq_t compute_f(idx_t n)
{
    if (n <= 2) return (seq_t)n;

    seq_t sigma = sigma_of_num(n);
    seq_t g = gcd((seq_t)n, sigma);
    return (g == (seq_t)1) ? (seq_t)0 : (seq_t)n;
}

/* Initialise the static buffers on first use. */
static void init_buffers(void)
{
    if (!init_done)
    {
        for (size_t i = 0; i <= MAX_VAL; ++i)
        {
            a_arr[i]      = 0;
            used_val[i]   = false;
            value_f[i]    = 0;
        }
        init_done = true;
    }
}

/* The required function. */
seq_t a(idx_t n)
{
    init_buffers();

    if (n == (idx_t)0) return (seq_t)0;          /* offset is 1 */

    /* Expand the computation up to n (order of calls arbitrary). */
    for (idx_t k = cur_n + 1; k <= n; ++k)
    {
        seq_t f_k = compute_f(k);

        /* Find the smallest admissible candidate value. */
        for (seq_t cand = 1; ; ++cand)
        {
            size_t v = (size_t)cand;
            if (v > MAX_VAL)    /* should not happen for n ≤ 10 000 */
            {
                /* If the candidate exceeds the buffer, bail out safely. */
                return (seq_t)0;
            }

            if (!used_val[v])
            {
                a_arr[k]      = cand;
                used_val[v]   = true;
                value_f[v]    = f_k;
                break;
            }
            else if (value_f[v] == f_k)
            {
                a_arr[k] = cand;
                break;
            }
            /* otherwise continue searching */
        }
    }
    cur_n = n;
    return a_arr[n];
}
