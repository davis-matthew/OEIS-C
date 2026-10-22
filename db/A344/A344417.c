/*
 * A344417  Number of palindromic factorizations of n.
 * https://oeis.org/A344417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* POI: Count palindromic factorizations by enumerating all unordered
   factorizations and accepting those whose factor multiset has at most
   one element with odd multiplicity.  */
#include <stdint.h>
#include <stddef.h>

/* forward declarations */
static int is_palindrome(const int *fac, size_t len);
static void dfs(idx_t rem, idx_t last, int *fac, size_t len, seq_t *res);

/* main sequence function --------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n <= 1u) return (seq_t)1;      /* a(1) = 1, also a(0) not used */

    /* stack for current factor multiset – max length 64 is ample for n≤10 000 */
    int fac[64];
    seq_t result = 0;

    dfs(n, 2u, fac, 0, &result);
    return result;
}

/* ------------------------------------------------------------------------- */
/*  Test whether the non‑decreasing multiset fac[0..len-1] can be permuted
    into a palindrome.  Essential property: at most one distinct value may
    occur an odd number of times. ----------------------------------------- */
static int is_palindrome(const int *fac, size_t len)
{
    int odd = 0;
    size_t i = 0;
    while (i < len) {
        int cur = fac[i];
        size_t j = i;
        while (j < len && fac[j] == cur) j++;      /* count frequency */
        if ( ((j - i) & 1) != 0 ) {                /* odd count?  */
            odd++;
            if (odd > 1) return 0;                 /* more than one odd → not palindrome */
        }
        i = j;
    }
    return 1;
}

/* ------------------------------------------------------------------------- */
/*  Depth‑first search enumerating unordered factorizations of ``rem``.
    ``last`` guarantees non‑decreasing order of factors, so each
    multiset is produced exactly once.  At the end (rem == 1) or when
    a factor equal to rem is chosen, the multiset in ``fac`` is tested.  */
static void dfs(idx_t rem, idx_t last, int *fac, size_t len, seq_t *res)
{
    if (rem == 1u) {
        if (is_palindrome(fac, len)) (*res)++;
        return;
    }

    /* iterate over divisors d of rem in the range [last, sqrt(rem)] */
    for (idx_t d = last; d <= rem / d; ++d) {
        if (rem % d == 0u) {
            fac[len] = (int)d;
            dfs(rem / d, d, fac, len + 1, res);
        }
    }

    /* handle the factor equal to rem itself (if permitted by order) */
    if (rem >= last) {
        fac[len] = (int)rem;
        if (is_palindrome(fac, len + 1)) (*res)++;
    }
}
