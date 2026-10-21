/*
 * A286308  Numbers m such that gcd(m, F(m)) = 2, where F(m) denotes the m-th
 *          Fibonacci number.
 * https://oeis.org/A286308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 4096, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, grow_seq, mul_mod64, seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A286308:
 *      a(n) = smallest integer m (in ascending order) such that gcd(m, F(m)) = 2
 *  (F is the Fibonacci sequence with F(0)=0, F(1)=1).
 *
 *  The routine generates the sequence on demand, caching the found terms so
 *  that subsequent calls are O(1).  For each new m it tests the condition
 *  using a fast‑doubling computation of F(m) modulo m, which guarantees
 *  correctness for both the 64‑bit and arbitrary‑precision builds.
 *
 *  Complexity: about O(n log n) for n terms.
 */

#include "../utils/gcd.h"
#include "../utils/mul_mod64.h"
#include "../utils/seq_alloc.h"
#include "../utils/grow_seq.h"

static seq_t *cache = NULL;         /* cached terms */
static size_t cache_cap = 0;        /* capacity of cache */
static size_t cache_len = 0;        /* number of cached terms */
static idx_t  m_checked = 0;        /* largest m processed */

/* fast doubling algorithm for F(n) modulo mod (mod >= 2) */
static uint64_t fib_mod_u64(idx_t n, uint64_t mod) {
    if (mod == 1) return 0;
    uint64_t a = 0;          /* F(0) */
    uint64_t b = 1;          /* F(1) */
    int bits = 63 - __builtin_clzll(n);
    for (int i = bits; i >= 0; --i) {
        /* Compute F(2k) and F(2k+1) from (a, b) = (F(k), F(k+1)) */
        uint64_t t1 = mul_mod64(2ULL, b, mod);
        uint64_t t2 = (t1 + mod - a) % mod;          /*  (2*F(k+1) - F(k)) mod mod */
        uint64_t c  = mul_mod64(a, t2, mod);          /* F(2k)   */
        uint64_t d  = (mul_mod64(a, a, mod) + mul_mod64(b, b, mod)) % mod; /* F(2k+1) */

        a = c;
        b = d;
        if ((n >> i) & 1ULL) {      /* if the i‑th bit of n is 1 */
            uint64_t e = (a + b) % mod;   /* F(2k+1) + F(2k+2) = F(2k+2) */
            a = b;
            b = e;
        }
    }
    return a;   /* F(n) mod mod */
}

seq_t a(idx_t n) {
    /* Ensure cache is at least n terms long */
    while (cache_len < (size_t)n) {
        ++m_checked;
        uint64_t m = (uint64_t)m_checked;
        uint64_t fmod = fib_mod_u64(m, m);
        seq_t g = gcd((seq_t)m, (seq_t)fmod);
        if (g == (seq_t)2) {
            if (cache_len == cache_cap)
                grow_seq(&cache, &cache_cap, cache_cap ? cache_cap * 2 : 8);
            cache[cache_len++] = m;
        }
    }
    return cache[n - 1];
}
