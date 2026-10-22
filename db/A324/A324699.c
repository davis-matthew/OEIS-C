/*
 * A324699  Lexicographically earliest sequence of positive integers whose
 *          prime indices minus 1 already belong to the sequence.
 * https://oeis.org/A324699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Lexicographically earliest sequence of positive integers whose
   prime indices minus 1 already belong to the sequence (OEIS A324699).
   The sequence is generated incrementally by testing candidate integers
   for admissibility.  A sieve of Eratosthenes is maintained to obtain
   prime factors and their 1‑based indices.  The implementation is fully
   self‑contained and works for any seq_t/idx_t combination provided by
   the build system.  Admissibility of a candidate `c` requires that for
   every prime factor `p` of `c`, the number `index(p)-1` is already
   present in the sequence.  The code does *not* hard‑code any terms; it
   recomputes them on demand and memoizes them for repeated queries.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* ----- Prime sieve (cached) ----- */
static uint32_t *primes = NULL;          /* array of primes */
static size_t     primes_cnt = 0;        /* number of primes */
static uint32_t *prime_to_index = NULL; /* prime → 1‑based index, size = sieve_limit+1 */
static idx_t     sieve_limit  = 0;       /* highest number sieved */

/* Build or extend the sieve so that it covers all numbers up to `limit`. */
static void build_sieve(idx_t limit)
{
    if (limit <= sieve_limit) return;          /* already sufficient */

    /* Allocate temporary marker array */
    uint8_t *is_prime = (uint8_t *)calloc((size_t)limit + 1, 1);
    if (!is_prime) abort();
    for (idx_t i = 2; i <= limit; ++i) is_prime[(size_t)i] = 1;

    /* Free previous data */
    free(primes);
    free(prime_to_index);

    /* Allocate containers for new sieve */
    primes_cnt = 0;
    primes = (uint32_t *)malloc((size_t)(limit / 10 + 32) * sizeof(uint32_t));
    if (!primes) abort();
    prime_to_index = (uint32_t *)calloc((size_t)limit + 1, sizeof(uint32_t));
    if (!prime_to_index) abort();

    uint32_t count = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_prime[(size_t)i]) continue;
        primes[count++] = (uint32_t)i;
        prime_to_index[(size_t)i] = count;  /* 1‑based index */

        if (i * i <= limit) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[(size_t)j] = 0;
        }
    }
    primes_cnt = count;
    sieve_limit = limit;
    free(is_prime);
}

/* ----- Sequence cache ----- */
static seq_t *seq      = NULL;    /* stored values */
static size_t seq_size = 0;       /* number of values stored */
static size_t seq_cap  = 0;       /* allocated capacity */

/* ----- Presence array for O(1) membership test ----- */
static uint8_t *present = NULL;   /* presence[x] == 1 if x in sequence */
static idx_t   present_limit = 0; /* highest value represented */

/* Ensure presence array covers values up to `val` (inclusive). */
static void ensure_present_upto(idx_t val)
{
    if (val <= present_limit) return;
    size_t new_limit = (size_t)val * 2;
    uint8_t *tmp = (uint8_t *)realloc(present, new_limit + 1);
    if (!tmp) abort();
    /* Zero new part */
    memset(tmp + present_limit + 1, 0, new_limit - present_limit);
    present = tmp;
    present_limit = new_limit;
}

/* ----- Admissibility test for a candidate ---------- */
static bool admissible(idx_t cand)
{
    idx_t x = cand;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes[i];
        if ((idx_t)p * (idx_t)p > x) break;
        if (x % p) continue;                    /* not a factor */

        /* Ensure we know the index of this prime */
        if ((idx_t)p > sieve_limit) build_sieve((idx_t)p * 2);
        uint32_t k = prime_to_index[(size_t)p];
        if (!k) return false;                /* prime outside sieve */
        if (!present[(size_t)k - 1]) return false; /* (k-1) missing */

        while (x % p == 0) x /= p;           /* fully remove p */
    }

    if (x > 1) { /* remaining factor is prime */
        if ((idx_t)x > sieve_limit) build_sieve((idx_t)x * 2);
        uint32_t k = prime_to_index[(size_t)x];
        if (!k) return false;
        if (!present[(size_t)k - 1]) return false;
    }
    return true;
}

/* ----- Initialization (lazy) ----- */
static void init(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    /* Build an initial sieve covering a reasonable range. */
    build_sieve(2000000);

    /* Allocate presence array lazily. */
    present_limit = 0;
    present = NULL;

    /* Allocate sequence array and insert first term 1. */
    seq_cap  = 16;
    seq      = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    if (!seq) abort();
    seq[0] = (seq_t)1;
    seq_size = 1;
    ensure_present_upto(1);
    present[1] = 1;
}

/* ----- Generate the next term of the sequence ----- */
static void generate_next(void)
{
    idx_t cand = (idx_t)seq[seq_size - 1] + 1;
    while (true) {
        ensure_present_upto(cand);
        if (admissible(cand)) {
            if (seq_size == seq_cap) {
                seq_cap <<= 1;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
                if (!seq) abort();
            }
            seq[seq_size++] = (seq_t)cand;
            present[(size_t)cand] = 1;
            return;
        }
        ++cand;
    }
}

/* ----- Public function required by the framework ----- */
seq_t a(idx_t n)
{
    init();
    if (n == 0) return (seq_t)0; /* undefined for n=0 */
    while (seq_size < (size_t)n) generate_next();
    return seq[n - 1];
}
