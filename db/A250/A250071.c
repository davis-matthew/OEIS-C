/*
 * A250071  Smallest number k such that the symmetric representation of
 *          sigma(k) has maximum width n for those k whose representation has
 *          nondecreasing width up to the diagonal.
 * https://oeis.org/A250071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min(2^m * q, m >= 0, q odd, sigma_0(q) = n, q < 2^(m+1))
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 30,
 *            big any n
 * Helpers:   bitlen_seq_t, pow2, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where sigma_0 is the divisor-count function.  The implementation
   precomputes, for a fixed N=10000, the smallest odd number q with
   exactly d divisors for all d ≤ N by exploring exponent patterns
   on odd primes.  For each n the answer a(n) is then obtained as
   2^⌊log2(q)⌋ * q.  The recursion respects the non‑increasing
   exponent rule so that the first time a divisor count is seen
   the value is minimal.  */

/* ---------- Standard headers ---------- */
#include <stdbool.h>
#include <stdint.h>

/* ---------- Utility headers ---------- */
#include "../utils/seq_mul.h"              /* seq_mul(seq_t a, seq_t b) */
#include "../utils/pow2.h"                 /* seq_t pow2(idx_t exp)   */
#include "../utils/bitlen_seq_t.h"         /* unsigned bitlen_seq_t(seq_t x) */

/* ---------- Constants ---------- */
#define MAX_N      10000          /* maximum divisor count to pre‑compute   */
#define MAX_EXP    30             /* upper limit for exponent in recursion */

/* ---------- Global data ---------- */
static seq_t best[MAX_N + 1];      /* best[d] = minimal odd number with d divisors */
static const uint32_t odd_primes[] = {3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59};
static const size_t PRIME_COUNT = sizeof(odd_primes) / sizeof(odd_primes[0]);

static bool precomputed = false;

/* ---------- Recursive generation of odd numbers with bounded divisor count ---------- */
static void dfs(int prime_idx, int prev_exp, seq_t val, idx_t divcnt)
{
    if (divcnt > MAX_N) return;
    if (best[divcnt] == 0 || val < best[divcnt])
        best[divcnt] = val;
    if (prime_idx >= (int)PRIME_COUNT) return;

    seq_t cur_val = val;
    for (int e = 1; e <= prev_exp && e <= MAX_EXP; ++e)
    {
        cur_val = seq_mul(cur_val, (seq_t)odd_primes[prime_idx]);   /* val * prime^e  */
        idx_t new_divcnt = divcnt * (e + 1);
        if (new_divcnt > MAX_N) break;
        dfs(prime_idx + 1, e, cur_val, new_divcnt);
    }
}

/* ---------- Precomputation routine ---------- */
static void precompute(void)
{
    for (size_t i = 0; i <= MAX_N; ++i) best[i] = 0;
    best[1] = 1;                                                /* q = 1 has 1 divisor */
    dfs(0, MAX_EXP, (seq_t)1, 1);
    precomputed = true;
}

/* ---------- Public sequence function ---------- */
seq_t a(idx_t n)
{
    if (!precomputed) precompute();

    if (n == 0) return 0;                                      /* safety, offset = 1 */

    seq_t q = best[(idx_t)n];                                  /* minimal odd q with n divisors */
    idx_t m = bitlen_seq_t(q);
    if (m == 0) return q;                                      /* shouldn't happen */
    m = m - 1;                                                 /* floor(log2(q)) */
    seq_t pow2m = pow2(m);
    return seq_mul(pow2m, q);                                   /* 2^m * q */
}
