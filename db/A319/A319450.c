/*
 * A319450  Numbers k such that k and k + 1 both have primitive roots.
 * https://oeis.org/A319450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth positive integer k such that both k and k+1 have
 *            a primitive root (i.e., k and k+1 are in OEIS A033948). The
 *            function generates terms on demand by testing membership in the
 *            set {1, 2, 4, p^e, 2·p^e} with p an odd prime
 * Range:     u8 n <= 35, u16 n <= 1019, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/* Small prime sieve – initialized once.                              */
/* ------------------------------------------------------------------ */
static uint32_t *prime_tbl = NULL;
static size_t   prime_cnt = 0;

static void init_primes(void)
{
    if (prime_tbl) return;                /* already initialized   */
    const size_t LIMIT = 1000000U;         /* more than enough       */
    bool *is_comp = (bool*)calloc(LIMIT + 1, sizeof(bool));
    if (!is_comp) return;                 /* allocation failure     */

    /* allocate rough storage */
    size_t approx = LIMIT / 10;
    prime_tbl = (uint32_t*)malloc(approx * sizeof(uint32_t));
    if (!prime_tbl) { free(is_comp); return; }

    prime_cnt = 0;
    for (size_t i = 2; i <= LIMIT; ++i) {
        if (!is_comp[i]) {
            prime_tbl[prime_cnt++] = (uint32_t)i;
            if (i * i <= LIMIT) {
                for (size_t j = i * i; j <= LIMIT; j += i)
                    is_comp[j] = true;
            }
        }
    }
    free(is_comp);
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n <= 3) return true;
    if ((n & 1ULL) == 0) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = prime_tbl[i];
        if ((uint64_t)p * (uint64_t)p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Checks whether n is an odd prime power p^e (p odd prime, e ≥ 1).   */
/* ------------------------------------------------------------------ */
static bool is_odd_prime_power(uint64_t n)
{
    if (n < 3) return false;          /* smallest odd prime power is 3 */
    if ((n & 1ULL) == 0) return false;/* must be odd                    */

    if (is_prime_uint64(n)) return true;

    uint64_t p = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t pr = prime_tbl[i];
        if ((uint64_t)pr * pr > n) break;
        if (n % pr == 0) { p = pr; break; }
    }
    if (!p) return false;            /* no prime factor found – should not happen */

    while (n % p == 0) n /= p;
    return n == 1;
}

/* ------------------------------------------------------------------ */
/* Determines whether n has a primitive root.                          */
/* Note: n is assumed to be a member of seq_t (possibly big).           */
/* We use a 64‑bit cast because all terms we test are ≤2^64 for the   */
/* required range of n.                                                */
/* ------------------------------------------------------------------ */
static bool has_primitive_root(seq_t n)
{
    if (n == (seq_t)1 || n == (seq_t)2 || n == (seq_t)4) return true;

    uint64_t v = (uint64_t)n;

    if ((v & 1ULL) == 0) {            /* n even → must be 2·p^e */
        return is_odd_prime_power(v >> 1);
    } else {                          /* n odd → must be p^e   */
        return is_odd_prime_power(v);
    }
}

/* ------------------------------------------------------------------ */
/* Cache of sequence values, grown lazily.                            */
/* ------------------------------------------------------------------ */
static idx_t  seq_len = 0;     /* number of terms stored      */
static size_t seq_cap = 0;     /* allocated capacity          */
static seq_t *seq_vals = NULL;

static void generate_terms(idx_t n)
{
    if (!seq_vals) {                     /* first allocation   */
        seq_cap = 128;
        seq_vals = (seq_t*)malloc(seq_cap * sizeof(seq_t));
    }

    static idx_t next_candidate = 1;
    while (seq_len < n) {
        seq_t k  = (seq_t)next_candidate;
        seq_t kp = k + 1;

        if (has_primitive_root(k) && has_primitive_root(kp)) {
            if (seq_len == seq_cap) {    /* grow array          */
                seq_cap *= 2;
                seq_t *tmp = (seq_t*)realloc(seq_vals, seq_cap * sizeof(seq_t));
                seq_vals = tmp;
            }
            seq_vals[seq_len++] = k;
        }
        ++next_candidate;
    }
}

/* ------------------------------------------------------------------ */
/* Exported sequence function.                                         */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* not part of the sequence */
    init_primes();                      /* ensure prime table ready  */
    generate_terms(n);
    return seq_vals[n - 1];             /* zero‑based indexing       */
}
