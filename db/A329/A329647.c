/*
 * A329647  Bitwise-XOR of exponents of prime factors of A108951(n), where
 *          A108951 is fully multiplicative with a(prime(i)) = prime(i)# =
 *          Product_{i=1..i} A000040(i).
 * https://oeis.org/A329647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7509, u16 n <= 7509, u32 n <= 7509, u64 n <= 7509,
 *            u128 n <= 7509, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS entry A329647
 *
 *  a(n) = bitwise XOR of the cumulative exponents of the prime factors
 *  of A108951(n).  For n = 1, a(1)=0.
 *
 *  A108951 is fully multiplicative with
 *      A108951(p_i) = primorial(p_i) = Π_{j≤i} p_j .
 *
 *  The exponent of prime p_j in A108951(n) = Σ_{i≥j} e_i
 *  where n = Π_i p_i^{e_i} .
 *  Thus a(n) = XOR_{j=1}^{maxIndex} ( Σ_{i≥j} e_i ).
 *
 *  The code is O(number of prime factors of n), well below the time
 *  limit for n ≤ 10 000.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/*  Prime table generation ------------------------------------------- */

#define PRIME_LIMIT   20000          /* enough for n ≤ 10 000 */
#define MAX_PRIMES    20000
#define MAX_INDEX     2000           /* max prime index needed for n ≤10k */

static int primes[MAX_PRIMES];
static int prime_count = 0;
static int prime_index[PRIME_LIMIT + 1] = {0};
static bool primes_built = false;

static void build_primes(void)
{
    if (primes_built)
        return;

    bool sieve[PRIME_LIMIT + 1];
    for (int i = 0; i <= PRIME_LIMIT; ++i)
        sieve[i] = true;
    sieve[0] = sieve[1] = false;

    for (int i = 2; i * i <= PRIME_LIMIT; ++i)
        if (sieve[i]) {
            for (int j = i * i; j <= PRIME_LIMIT; j += i)
                sieve[j] = false;
        }

    for (int i = 2; i <= PRIME_LIMIT; ++i)
        if (sieve[i]) {
            primes[prime_count++] = i;
            prime_index[i] = prime_count;   /* 1‑based index */
        }

    primes_built = true;
}

/* ------------------------------------------------------------------ */
/*  Factorisation of an integer ≤ 10 000 ------------------------------ */

static void factor_uint64(uint64_t n, int *idx, int *exp, int *cnt)
{
    *cnt = 0;
    for (int i = 0; i < prime_count && (uint64_t)primes[i] * primes[i] <= n; ++i) {
        int p = primes[i];
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            idx[(*cnt)] = prime_index[p];
            exp[(*cnt)] = e;
            ++(*cnt);
        }
    }
    if (n > 1) {               /* remaining prime factor > sqrt(original n) */
        int p = (int)n;
        idx[(*cnt)] = prime_index[p];
        exp[(*cnt)] = 1;
        ++(*cnt);
    }
}

/* ------------------------------------------------------------------ */
/*  Sequence function ----------------------------------------------- */

/*  All arrays below are fixed‑size; no variable‑length arrays are used. */
static uint32_t add[MAX_INDEX + 1];

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n<1, return 0 for safety */

    build_primes();

    /* Factorise n */
    int f_idx[16], f_exp[16], f_cnt;
    factor_uint64((uint64_t)n, f_idx, f_exp, &f_cnt);

    if (f_cnt == 0)
        return (seq_t)0;   /* n == 1 */

    /* Compute maximum prime index among the factors */
    int max_idx = 0;
    uint32_t total_exp = 0;
    for (int i = 0; i < f_cnt; ++i) {
        if (f_idx[i] > max_idx)
            max_idx = f_idx[i];
        total_exp += (uint32_t)f_exp[i];
    }

    if (max_idx > MAX_INDEX) {
        /* This should not happen for n ≤ 10 000, but clamp to limit. */
        max_idx = MAX_INDEX;
    }

    /* Initialise additive array for this call */
    memset(add, 0, (max_idx + 1) * sizeof(uint32_t));

    /* Accumulate exponents per prime index */
    for (int i = 0; i < f_cnt; ++i)
        add[f_idx[i]] += (uint32_t)f_exp[i];

    /* Compute cumulative sums from the top index downwards and XOR them */
    uint32_t prefix = 0;
    seq_t result = 0;
    for (int j = 1; j <= max_idx; ++j) {
        uint32_t cur_sum = total_exp - prefix;
        if (cur_sum != 0)
            result ^= (seq_t)cur_sum;
        prefix += add[j];
    }
    return result;
}
