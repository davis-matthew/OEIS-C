/*
 * A330883  A variation of the 'viral sequence' A334355; the same rules for
 *          a(n) apply but all numbers must be prime.
 * https://oeis.org/A330883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 3752, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 8629 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computes OEIS sequence A330883.
 *
 * The sequence is defined recursively:
 *   a(1) = 2
 *   For n ≥ 2, choose the smallest unused prime p such that
 *     s = a(n-1) + p is composite, and
 *     if n is odd then s+1 is prime, else s+1 is composite.
 *
 * The implementation builds the sequence incrementally, storing the
 * primes that have appeared and a flag indicating whether each prime
 * has already been used in the sequence.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

static seq_t *seq = NULL;          /* computed sequence values */
static size_t seq_cap = 0;
static size_t seq_len = 0;         /* number of elements currently stored */

static seq_t *primes = NULL;       /* list of all primes discovered */
static bool *used = NULL;          /* used[i] == true iff primes[i] is already in seq */
static size_t primes_cap = 0;
static size_t primes_len = 0;

static seq_t next_prime_candidate = (seq_t)2;   /* integer to test for next prime */

/* Ensure seq has space for at least 'needed' elements */
static void ensure_seq_capacity(size_t needed)
{
    if (seq_cap < needed) {
        size_t newcap = seq_cap > 0 ? seq_cap * 2 : 16;
        while (newcap < needed) newcap *= 2;
        seq = mem_resize(seq_t, seq, newcap);
        seq_cap = newcap;
    }
}

/* Ensure primes and used arrays have capacity for at least one more element */
static void ensure_primes_capacity(void)
{
    if (primes_cap == 0) {
        primes_cap = 16;
        primes = mem_new(seq_t, primes_cap);
        used  = (bool  *)malloc(primes_cap * sizeof(bool));
    } else if (primes_len >= primes_cap) {
        primes_cap *= 2;
        primes = mem_resize(seq_t, primes, primes_cap);
        used  = (bool  *)realloc(used,  primes_cap * sizeof(bool));
    }
}

/* Add a new prime to the primes list */
static void add_prime(seq_t p)
{
    ensure_primes_capacity();
    primes[primes_len] = p;
    used[primes_len]   = false;
    primes_len++;
}

/* Generate the next prime greater than or equal to next_prime_candidate */
static void generate_next_prime(void)
{
    for (;;) {
        if (is_prime_big(next_prime_candidate)) {
            add_prime(next_prime_candidate);
            next_prime_candidate += (seq_t)1;
            break;
        }
        next_prime_candidate += (seq_t)1;
    }
}

/* Ensure that primes[0 .. idx] exist */
static void ensure_prime_index(size_t idx)
{
    while (primes_len <= idx)
        generate_next_prime();
}

/* Generate sequence values up to (and including) index n (1‑based) */
static void generate_up_to(idx_t n)
{
    if (seq_len == 0) {
        /* initialise a(1) = 2 */
        ensure_prime_index(0);                /* guarantees primes[0]==2 */
        ensure_seq_capacity(1);
        seq_len = 1;
        seq[0] = primes[0];
        used[0] = true;
    }

    for (size_t cur = seq_len; cur < (size_t)n; ++cur) {
        seq_t prev = seq[cur - 1];
        size_t pidx = 0;

        for (;;) {
            ensure_prime_index(pidx);
            if (used[pidx]) { ++pidx; continue; }

            seq_t  p   = primes[pidx];
            seq_t  sum = prev + p;

            /* a(n-1) + a(n) must be composite */
            if (is_prime_big(sum)) { ++pidx; continue; }

            seq_t  sum1 = sum + (seq_t)1;
            /* alternating condition on sum+1 */
            bool ok = ((cur + 1) % 2 == 1) ? is_prime_big(sum1) : !is_prime_big(sum1);

            if (ok) {               /* found suitable prime */
                ensure_seq_capacity(cur + 1);
                seq[cur]     = p;
                used[pidx]   = true;
                break;
            }

            ++pidx;
        }
        seq_len++;
    }
}

/* Public entry point */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    generate_up_to(n);
    return seq[n - 1];
}
