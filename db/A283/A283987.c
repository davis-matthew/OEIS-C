/*
 * A283987  a(n) = A002487(n-1) XOR A002487(n), where XOR is bitwise-xor
 *          (A003987).
 * https://oeis.org/A283987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A002487(n-1) XOR A002487(n) (Stern's diatomic sequence)
 * Range:     u8 n <= 4690, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached Stern’s diatomic sequence:  S[0] = 0, S[1] = 1, S[2k] = S[k], S[2k+1] = S[k] + S[k+1] */
static seq_t *Stern = NULL;    /* array of Stern values (index from 0) */
static idx_t  SternSize = 0;   /* allocated array size (number of elements) */
static idx_t  SternMax  = 0;   /* largest index that has been computed */

/* Ensure the array has room for indices up to n (inclusive) */
static void ensure_stern_cap(idx_t n)
{
    if (SternSize > n)
        return;                  /* already large enough */

    /* Allocate a larger buffer; newSize = n+1 so index n is valid */
    size_t newSize = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(Stern, newSize * sizeof(seq_t));
    if (!tmp)                     /* abort if out‑of‑memory */
        exit(1);
    Stern = tmp;
    SternSize = newSize;
}

/* Compute Stern values up to index n (inclusive) and store them in Stern[] */
static void grow_stern(idx_t n)
{
    if (n <= SternMax)
        return;                  /* nothing to do */

    /* First time we compute the sequence, initialise the base elements */
    if (SternMax == 0) {
        if (SternSize < 2)       /* array must hold indices 0 and 1 */
            ensure_stern_cap(1);
        Stern[0] = (seq_t)0;
        Stern[1] = (seq_t)1;
        SternMax  = 1;
    }

    /* Generate values for indices SternMax+1 … n */
    for (idx_t i = SternMax + 1; i <= n; ++i) {
        if ((i & 1) == 0) {           /* even: S[i] = S[i/2] */
            Stern[i] = Stern[i >> 1];
        } else {                      /* odd:  S[i] = S[k] + S[k+1] where k = (i-1)/2 */
            idx_t k = i >> 1;
            Stern[i] = Stern[k] + Stern[k + 1];
        }
    }
    SternMax = n;
}

seq_t a(idx_t n)
{
    /* n must be >= 1 (offset 1).  For n==0 behaviour is not specified. */
    if (n == 0)
        return (seq_t)0;          /* graceful fallback */

    ensure_stern_cap(n);
    grow_stern(n);
    /* a(n) = Stern(n-1) XOR Stern(n) */
    return Stern[n - 1] ^ Stern[n];
}
