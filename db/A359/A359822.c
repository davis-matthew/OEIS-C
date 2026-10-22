/*
 * A359822  Numbers k whose arithmetic derivative, A003415(k), has the same
 *          parity as k.
 * https://oeis.org/A359822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers k whose arithmetic derivative has the same parity as k.
   a(1) = 0, a(2) = 3, a(3) = 4, a(4) = 5, a(5) = 7, ... */

/* Compute the arithmetic derivative of n.
   Uses the identity: D(n) = ∑_{p^e | n} e * n / p  */
static seq_t arithmetic_derivative(uint64_t n)
{
    if (n <= 1) return 0;
    seq_t n_seq = (seq_t)n;
    seq_t der = 0;
    uint64_t temp = n;

    /* factor 2 */
    if ((temp & 1ULL) == 0) {
        int e = 0;
        while ((temp & 1ULL) == 0) { temp >>= 1; ++e; }
        der += (n_seq / (seq_t)2) * (seq_t)e;
    }

    /* odd factors */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) { temp /= p; ++e; }
            der += (n_seq / (seq_t)p) * (seq_t)e;
        }
    }

    if (temp > 1) {          /* remaining prime factor */
        der += (n_seq / (seq_t)temp) * (seq_t)1;
    }

    return der;
}

/* Fixed-size cache for the first 20000 terms (enough for up to n=10000). */
#define MAX_TERMS 20000
static seq_t cache[MAX_TERMS];
static idx_t cache_size = 0;
static uint64_t next_candidate = 0;

/* Generate terms until we have at least n of them. */
static void ensure_cache(idx_t n)
{
    if (cache_size >= n) return;
    while (cache_size < n) {
        uint64_t k = next_candidate;
        seq_t der = arithmetic_derivative(k);
        /* parity check */
        if (((k & 1ULL) == (uint64_t)(der & (seq_t)1)) && cache_size < MAX_TERMS) {
            cache[cache_size++] = (seq_t)k;
        }
        ++next_candidate;
    }
}

/* External linkage as required. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined, but harmless */
    ensure_cache(n);
    return cache[n - 1];
}
