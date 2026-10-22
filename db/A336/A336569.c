/*
 * A336569  Number of maximal strict chains of divisors from n to 1 using
 *          elements of A130091 (numbers with distinct prime multiplicities).
 * https://oeis.org/A336569
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

/* Computation: a(1)=1; for n>1, a(n)=0 if n∉S (S=A130091), otherwise
 * a(n)=∑ a(d) over all maximal elements d∈S dividing n, d<n.
 * An element d is maximal iff no other element e∈S, e>d, e|n, d|e.
 * The algorithm pre‑computes the smallest prime factor array, the
 * membership array of S, and the DP table up to the requested n.
 * All computations use the build‑supplied type seq_t.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* static state */
static idx_t   *spf          = NULL;    /* smallest prime factor */
static bool    *is_S         = NULL;    /* true iff number has distinct exponents */
static seq_t   *dp           = NULL;    /* number of maximal chains for each n */
static idx_t    global_limit = 0;        /* maximum index currently prepared */

/* compute smallest prime factor array up to limit */
static void calc_spf(idx_t limit)
{
    for (idx_t i = 0; i <= limit; ++i) spf[i] = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            for (idx_t j = i; j <= limit; j += i) {
                if (spf[j] == 0) spf[j] = i;
            }
        }
    }
}

/* determine membership in A130091 up to limit */
static void calc_is_S(idx_t limit)
{
    is_S[1] = true;               /* 1 is considered to have distinct exponents */
    for (idx_t n = 2; n <= limit; ++n) {
        idx_t x = n;
        bool ok = true;
        int seen[64] = {0};
        int cnt = 0;
        while (x > 1 && ok) {
            idx_t p = spf[x];
            int exp = 0;
            while (x % p == 0) {
                x /= p;
                ++exp;
            }
            for (int i = 0; i < cnt && ok; ++i) {
                if (seen[i] == exp) ok = false;
            }
            if (ok) seen[cnt++] = exp;
        }
        is_S[n] = ok;
    }
}

/* compute dp table up to limit */
static void calc_dp(idx_t limit)
{
    dp[1] = (seq_t)1;      /* base case */
    for (idx_t n = 2; n <= limit; ++n) {
        if (!is_S[n]) {          /* non‑member starts a chain when n∉S */
            dp[n] = (seq_t)0;
            continue;
        }

        /* gather all S-divisors of n smaller than n */
        int divs[256];
        int dcnt = 0;
        for (idx_t j = 1; j * j <= n; ++j) {
            if (n % j == 0) {
                idx_t d1 = j, d2 = n / j;
                if (d1 < n && is_S[d1]) divs[dcnt++] = (int)d1;
                if (d2 < n && d2 != d1 && is_S[d2]) divs[dcnt++] = (int)d2;
            }
        }

        /* filter maximal elements */
        bool allow[256];
        for (int i = 0; i < dcnt; ++i) allow[i] = true;
        for (int i = 0; i < dcnt; ++i) {
            if (!allow[i]) continue;
            for (int j = 0; j < dcnt; ++j) {
                if (i == j) continue;
                if (divs[j] > divs[i] && divs[j] % divs[i] == 0) {
                    allow[i] = false;
                    break;
                }
            }
        }

        /* sum dp over maximal divisors */
        seq_t total = 0;
        for (int i = 0; i < dcnt; ++i)
            if (allow[i]) total += dp[(idx_t)divs[i]];
        dp[n] = total;
    }
}

/* ensure global arrays cover up to n (recompute all data up to n) */
static void ensure_limit(idx_t n)
{
    if (n <= global_limit) return;
    idx_t new_limit = n + 10;   /* small cushion */

    spf  = (idx_t *)realloc(spf,  (new_limit + 1) * sizeof(idx_t));
    is_S = (bool  *)realloc(is_S, (new_limit + 1) * sizeof(bool));
    dp   = (seq_t *)realloc(dp,   (new_limit + 1) * sizeof(seq_t));
    if (!spf || !is_S || !dp) abort();

    calc_spf(new_limit);
    calc_is_S(new_limit);
    calc_dp(new_limit);

    global_limit = new_limit;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset starts at 1 */
    ensure_limit(n);
    return dp[(idx_t)n];
}
