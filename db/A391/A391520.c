/*
 * A391520  Square array T(n,k) read by ascending antidiagonals: T(n,k) = (p -
 *          1)/2*(2*(n - 1)*p^(n - 1) - (2*n - 3)*p^(n - 2)), n>=2, where p is
 *          the k-th odd prime.
 * https://oeis.org/A391520
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   T(n,k) = (p-1)/2*( 2*(n-1)*p^(n-1) - (2*n-3)*p^(n-2) ), n≥2 a(n)
 *            = T(i,j) read by ascending antidiagonals, with p the k‑th odd
 *            prime. Off‑set: a(2) is the first listed value
 * Range:     u8 n <= 7, u16 n <= 23, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Helper to build a list of primes up to a given limit. */
#include "../utils/build_primes_upto.h"

static uint32_t *odd_primes = NULL;   /* only odd primes, 3,5,7, ... */
static size_t    odd_cnt   = 0;       /* number of odd primes stored */

/* Allocate the odd‑prime list (called only once). */
static void init_odd_primes(void)
{
    if (odd_primes != NULL)
        return;                     /* already initialized */

    const idx_t limit = 2000;        /* guaranteed > 139th odd prime */
    uint64_t *primes = NULL;
    size_t cnt = 0;

    /* build_primes_upto: primes[0] = 2 */
    build_primes_upto(limit, &primes, &cnt);

    odd_cnt  = cnt - 1;             /* exclude the prime 2 */
    odd_primes = (uint32_t*)malloc(odd_cnt * sizeof(uint32_t));

    size_t j = 0;
    for (size_t i = 0; i < cnt; ++i)
        if (primes[i] != 2ull)
            odd_primes[j++] = (uint32_t)primes[i];

    free(primes);
}

/* 128‑bit fast exponentiation: base^exp.  exp is small (≤ ~140). */
static unsigned __int128 pow128(uint64_t base, idx_t exp)
{
    unsigned __int128 res = 1;
    unsigned __int128 b   = (unsigned __int128)base;
    while (exp)
    {
        if (exp & 1U)
            res *= b;
        b *= b;
        exp >>= 1U;
    }
    return res;
}

/* Main sequence function.  External linkage is guaranteed by build. */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;          /* undefined below the offset */

    init_odd_primes();        /* ensure odd primes are ready */

    /* --- decode the global index into (row n, column k) ---------------- */
    idx_t m   = n - 2;        /* zero‑based index into the array */
    idx_t s   = 3;            /* current diagonal sum (n + k) */
    idx_t cum = 0;            /* cumulative terms of previous diagonals */

    /* skip whole diagonals until we reach the one containing m */
    while (cum + (s - 2) <= m)
    {
        cum += (s - 2);
        ++s;
    }

    idx_t offset = m - cum;                 /* position within diagonal */
    idx_t row    = (s - 1) - offset;        /* n */
    idx_t col    = s - row;                 /* k, 1‑based */

    uint32_t p = odd_primes[col - 1];       /* k‑th odd prime */

    /* --- compute T(row, col) using 128‑bit intermediate values ---------- */
    unsigned __int128 factor = ((unsigned __int128)(p - 1)) >> 1;   /* (p-1)/2 */

    unsigned __int128 pow1 = (row >= 2) ? pow128(p, row - 1) : 1;
    unsigned __int128 pow2 = (row >= 3) ? pow128(p, row - 2) : 1;

    unsigned __int128 inner =
        2 * (unsigned __int128)(row - 1) * pow1
      - (2 * (unsigned __int128)row - 3) * pow2;

    unsigned __int128 result = factor * inner;

    return (seq_t)result;
}
