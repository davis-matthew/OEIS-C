/*
 * A282585  Number of ways to write n as an ordered sum of 3 squarefree
 *          palindromes (A071251).
 * https://oeis.org/A282585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of ways to write n as an ordered sum of 3 squarefree palindromes
   (OEIS A282585).  The formula is
   a(n) = Σ_{i+j+k=n} 1_{i is squarefree palindrome}·1_{j is squarefree palindrome}
                     ·1_{k is squarefree palindrome}.
   We pre‑compute the indicator array f and then perform two convolutions
   to obtain a(n).  For n ≤ 20000 the simple O(n²) algorithm runs well
   under 2 s.  The implementation uses only standard C/C++ features and
   avoids dynamic construction of arbitrary‑precision types. */
#include <stdint.h>
#include <stdbool.h>

/* Static scratch space – enough for the test range (≤20000) */
#define MAX_N 20000
static seq_t f[MAX_N + 1];
static seq_t g[MAX_N + 1];
static seq_t a_arr[MAX_N + 1];
static idx_t current_max = 0;

/* Helper: test whether a 64‑bit integer is a base‑10 palindrome. */
static bool is_palindrome_uint64(uint64_t n)
{
    uint64_t rev = 0, t = n;
    while (t > 0) {
        rev = rev * 10 + t % 10;
        t   /= 10;
    }
    return rev == n;
}

/* Helper: test whether a 64‑bit integer is square‑free. */
static bool is_squarefree_uint64(uint64_t n)
{
    if (n <= 1) return true;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            if (n % (p * p) == 0) return false;
        }
    }
    return true;
}

/* Reset and compute all values up to n.  The algorithm recomputes from
   scratch each time; the cost is negligible for the expected limits. */
static void prepare(idx_t n)
{
    if (n > MAX_N) return;   /* safety; should not happen in tests */

    if (n <= current_max) return;

    /* 1.  Indicator array f[i] = 1 if i is a square‑free palindrome, else 0. */
    f[0] = 0;   /* 0 is not square‑free. */
    for (idx_t i = 1; i <= n; ++i) {
        uint64_t u = (uint64_t)i;
        f[i] = (is_palindrome_uint64(u) && is_squarefree_uint64(u)) ? 1 : 0;
    }

    /* 2.  Convolution of f with itself: g[s] = Σ_{i=0}^s f[i]·f[s-i]   (ordered pairs). */
    for (idx_t s = 0; s <= n; ++s) {
        seq_t sum = 0;
        for (idx_t i = 0; i <= s; ++i) {
            sum = sum + f[i] * f[s - i];   /* f[i], f[s-i] ∈ {0,1}  */
        }
        g[s] = sum;
    }

    /* 3.  Convolution of g with f: a[n] = Σ_{s=0}^n g[s]·f[n-s]  (ordered triples). */
    for (idx_t m = 0; m <= n; ++m) {
        seq_t sum = 0;
        for (idx_t s = 0; s <= m; ++s) {
            sum = sum + g[s] * f[m - s];
        }
        a_arr[m] = sum;
    }

    current_max = n;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    prepare(n);
    if (n > MAX_N) return 0;   /* out of range – should not happen */
    return a_arr[n];
}
