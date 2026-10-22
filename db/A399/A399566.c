/*
 * A399566  Number of k such that A046073(k) = n.
 * https://oeis.org/A399566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, new
 * Formula:   a(n) = Number of k such that A046073(k) = n. A046073(k) is the
 *            number of squares in the multiplicative group (Z/kZ)*. For
 *            k=2^e·∏p_i^{e_i} (p_i odd primes) A046073(k) = 2^{max(e-3,0)} ·
 *            ∏ [p_i^{e_i-1} · (p_i-1)/2] The algorithm enumerates all
 *            factorizations of n = 2^s · t (t odd) into the contrib...
 * Range:     u8 n <= 989, u16 n <= 1889, u32 n <= 1889, u64 n <= 1889,
 *            u128 n <= 1889, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Max n is 10000; primes up to 10000 are enough.  */
#define MAX_N 10000
#define MAX_PRIMES 1229   /* number of primes <= 10000 */

/* structures to hold per‑prime precomputation */
typedef struct {
    uint64_t p;     /* odd prime */
    unsigned shift; /* v2((p-1)/2) */
    uint64_t dodd;  /* odd part of (p-1)/2 */
} prime_info_t;

/* global data */
static prime_info_t prime_list[MAX_PRIMES];
static size_t prime_cnt = 0;
static unsigned g_s = 0;    /* power of two in current n */

/* helper: count trailing zeros in 64‑bit value */
static unsigned trailing_zeros_uint64(uint64_t x)
{
    /* x is guaranteed >0 */
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_ctzll(x);
#else
    unsigned c = 0;
    while ((x & 1ULL) == 0) {
        c++;
        x >>= 1;
    }
    return c;
#endif
}

/* recursion: count solutions for current n */
static seq_t dfs(uint64_t rem, size_t idx, unsigned sum)
{
    /* base: all primes considered? */
    if (idx == prime_cnt) {
        if (rem == 1) {
            if (g_s < sum) return (seq_t)0;
            unsigned diff = g_s - sum;
            if (diff == 0) return (seq_t)4;   /* e₂ = 0,1,2,3 */
            else return (seq_t)1;             /* e₂ fixed: e₂ = diff + 3 */
        }
        return (seq_t)0;
    }

    seq_t res = dfs(rem, idx + 1, sum);  /* skip this prime */

    /* try to use this prime with exponents e >= 1 */
    const prime_info_t *pr = &prime_list[idx];
    uint64_t pow = 1;  /* p^{e-1} */
    for (;;) {
        /* val = p^{e-1} * dodd */
        if (pow > rem / pr->dodd) break;
        uint64_t val = pow * pr->dodd;
        if (rem % val == 0)
            res += dfs(rem / val, idx + 1, sum + pr->shift);
        if (pow > rem / pr->p) break;
        pow *= pr->p;
    }
    return res;
}

/* build prime list (called once) */
static void init_prime_list(void)
{
    /* simple sieve up to MAX_N */
    bool is_composite[MAX_N + 1];
    for (size_t i = 0; i <= MAX_N; ++i) is_composite[i] = false;
    for (size_t i = 3; i * i <= MAX_N; i += 2) {
        if (!is_composite[i]) {
            for (size_t j = i * i; j <= MAX_N; j += i << 1)
                is_composite[j] = true;
        }
    }
    for (size_t p = 3; p <= MAX_N; p += 2) {
        if (!is_composite[p]) {
            uint64_t d = (p - 1) / 2;
            unsigned shift = trailing_zeros_uint64(d);
            uint64_t dodd = d >> shift;
            prime_list[prime_cnt].p = p;
            prime_list[prime_cnt].shift = shift;
            prime_list[prime_cnt].dodd = dodd;
            prime_cnt++;
        }
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* factor out powers of two */
    unsigned s = 0;
    uint64_t rem = (uint64_t)n;
    while ((rem & 1ULL) == 0) {
        rem >>= 1;
        s++;
    }
    /* rem now contains odd part t */
    if (prime_cnt == 0) init_prime_list();
    g_s = s;
    return dfs(rem, 0, 0);
}
