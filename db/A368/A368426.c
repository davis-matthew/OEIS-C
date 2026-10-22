/*
 * A368426  Centered 10-gonal numbers which are sphenic numbers.
 * https://oeis.org/A368426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 5, u32 n <= 5080, u64 n <= 5080,
 *            u128 n <= 5080, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the nth centered 10‑gonal number that is sphenic (product of
   three distinct primes).  The sequence is obtained by generating the
   centered 10‑gonal numbers C10(m)=5*m*(m-1)+1 and selecting those
   which are sphenic.  The function memoises results so repeated
   calls are efficient. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* forward declaration of the sequence function */
seq_t a(idx_t n);

/* ---------- helper: prime sieve up to a fixed limit ---------- */

static uint32_t *primes = NULL;
static size_t primes_cnt = 0;

static void init_primes(void)
{
    if (primes != NULL)
        return;                         /* already initialised */

    /* A safe upper bound: the 10thᵗʰ centered number for n≈10⁶
       is ≈5·10¹², so sqrt(max)≈7·10⁵.  We use 2 000 000 to be safe. */
    const int LIMIT = 2000000;
    static unsigned char *sieve = NULL;
    static size_t sieve_cap = 0;

    /* allocate sieve array once */
    if (sieve == NULL) {
        sieve_cap = LIMIT + 1;
        sieve = (unsigned char *)malloc(sieve_cap);
        if (!sieve)
            abort();                    /* memory allocation failure */
        for (size_t i = 0; i < sieve_cap; ++i)
            sieve[i] = 1;
        sieve[0] = sieve[1] = 0;
        for (int i = 2; i * i <= LIMIT; ++i) {
            if (sieve[i]) {
                for (int j = i * i; j <= LIMIT; j += i)
                    sieve[j] = 0;
            }
        }
        /* count primes to allocate storage */
        for (int i = 2; i <= LIMIT; ++i)
            if (sieve[i])
                ++primes_cnt;

        primes = (uint32_t *)malloc(primes_cnt * sizeof(uint32_t));
        if (!primes)
            abort();                    /* memory allocation failure */
        size_t pos = 0;
        for (int i = 2; i <= LIMIT; ++i)
            if (sieve[i])
                primes[pos++] = (uint32_t)i;
        /* free sieve to reclaim RAM */
        free(sieve);
        sieve = NULL;
    }
}

/* ---------- test if a 64‑bit unsigned integer is sphenic ---------- */

static bool is_sphenic(uint64_t x)
{
    size_t cnt = 0;
    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * primes[i] <= x; ++i) {
        if (x % primes[i] == 0) {
            size_t exp = 0;
            while (x % primes[i] == 0) {
                x /= primes[i];
                ++exp;
                if (exp > 1)          /* not square‑free */
                    return false;
            }
            ++cnt;
            if (cnt > 3)
                return false;
        }
    }
    if (x > 1) {                         /* remaining prime factor */
        ++cnt;
        if (cnt > 3)
            return false;
    }
    return cnt == 3;                     /* exactly three distinct primes */
}

/* ---------- memoised cache of sequence terms ---------- */

static seq_t *cache      = NULL;
static size_t cache_cap  = 0;            /* allocated capacity */
static size_t cache_len  = 0;            /* number of terms stored */

static idx_t last_tested = 0;            /* largest n already examined */

/* ---------- main sequence function ---------- */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* offset is 1; undefined below 1 */

    /* ensure primes are ready */
    init_primes();

    /* grow cache storage if necessary */
    if (cache_len == 0) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            abort();
    }

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        ++last_tested;                 /* next candidate n */
        seq_t val = 5 * (seq_t)last_tested *
                    ((seq_t)last_tested - 1) + 1;  /* centered 10‑gonal */

        /* test for sphenic using 64‑bit arithmetic */
        if (is_sphenic((uint64_t)val)) {
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(cache,
                                             cache_cap * sizeof(seq_t));
                if (!tmp)
                    abort();
                cache = tmp;
            }
            cache[cache_len++] = val;
        }
    }
    return cache[n - 1];                /* a(1) is cache[0] */
}
