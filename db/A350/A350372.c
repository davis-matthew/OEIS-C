/*
 * A350372  Numbers with exactly 5 semiprime divisors.
 * https://oeis.org/A350372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute A350372: numbers with exactly 5 semiprime divisors.
    The algorithm factorises each integer, counts the number
    of distinct prime factors k and the number of primes with
    exponent >= 2 (t), then checks whether C(k,2)+t == 5.
    Sequence terms are generated incrementally and cached. */

#include <stdint.h>
#include <stdlib.h>

/* forward defaults are provided by the build system;
   we simply use the types seq_t and idx_t. */

/* --------------------- prime sieve --------------------- */
#if 0
/* dummy definition; the build supplies these types */
typedef uint64_t idx_t;
typedef unsigned __int128 seq_t;
#endif

/* maximum prime needed for trial division.
   1e6 is enough for the first 10000 terms.  */
#define PRIME_SIEVE_LIMIT 1000000U

static uint32_t *prime_list = NULL;   /* array of primes */
static size_t prime_count = 0;        /* number of primes found */

/* initializes prime_list once */
static void init_primes(void)
{
    if (prime_list != NULL) return;          /* already built */

    /* simple sieve of Eratosthenes using a bit array */
    uint32_t limit = PRIME_SIEVE_LIMIT;
    uint8_t *mark = (uint8_t *)malloc(limit + 1);
    if (!mark) return;
    for (uint32_t i = 0; i <= limit; ++i) mark[i] = 1;
    mark[0] = mark[1] = 0;

    for (uint32_t i = 2; i * i <= limit; ++i)
        if (mark[i])
            for (uint32_t j = i * i; j <= limit; j += i)
                mark[j] = 0;

    /* count primes first */
    size_t cnt = 0;
    for (uint32_t i = 2; i <= limit; ++i)
        if (mark[i]) ++cnt;

    /* allocate buffer and fill */
    prime_list = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!prime_list) { free(mark); prime_list = NULL; return; }
    for (uint32_t i = 2; i <= limit; ++i)
        if (mark[i]) prime_list[prime_count++] = i;

    free(mark);
}

/* ----------------------- sequence cache ----------------------- */
static seq_t *seq_cache = NULL;    /* dynamic array of sequence terms */
static size_t seq_len = 0;         /* number of terms stored */
static size_t seq_cap = 0;         /* allocated capacity */

/* append a new term to seq_cache */
static void append_term(seq_t val)
{
    if (seq_len == seq_cap) {
        seq_cap = seq_cap ? (seq_cap << 1) : 1024;
        seq_cache = (seq_t *)realloc(seq_cache, seq_cap * sizeof(seq_t));
    }
    seq_cache[seq_len++] = val;
}

/* ----------------- semiprime divisor count ----------------- */
static inline size_t semiprime_divisor_count(uint64_t n)
{
    size_t k = 0;   /* distinct prime factors */
    size_t t = 0;   /* primes with exponent >= 2 */

    uint64_t temp = n;
    for (size_t i = 0; i < prime_count && (uint64_t)prime_list[i] * prime_list[i] <= temp; ++i) {
        uint32_t p = prime_list[i];
        if (temp % p == 0) {
            size_t exp = 0;
            do {
                temp /= p;
                ++exp;
            } while (temp % p == 0);
            ++k;
            if (exp >= 2) ++t;
        }
    }
    if (temp > 1) {
        /* remaining prime factor with exponent 1 */
        ++k;
    }
    return (k * (k - 1) / 2) + t;
}

/* ----------------- sequence generator ------------------- */
static uint64_t next_candidate = 2;    /* next integer to test */

static void ensure_sequence(idx_t target)
{
    if (seq_len >= target) return;   /* already enough terms */

    init_primes();                   /* ensure primes ready */

    while (seq_len < target) {
        size_t s = semiprime_divisor_count(next_candidate);
        if (s == 5) {
            append_term((seq_t)next_candidate);
        }
        ++next_candidate;
    }
}

/* --------------------- public function ------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, return 0 for safety */
    ensure_sequence(n);
    return seq_cache[n - 1];
}
