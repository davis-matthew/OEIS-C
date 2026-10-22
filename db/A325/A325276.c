/*
 * A325276  Irregular triangle read by rows where row n is the omega-sequence
 *          of n!.
 * https://oeis.org/A325276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the nth term in the flattened triangle read by rows
 *            where row r (starting at 0) is the omega‑sequence of (r+2)!. The
 *            omega‑sequence of a number m is defined as follows: term 0 =
 *            Omega(m) for k>0 : term k = Omega(red^k(m)) where red(n = p1^e1
 *            * ... * pk^ek) = prime(e1) * ... * prime(ek) an...
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* limit for sieve: enough to provide prime(e) up to e≈200000
   (the exponent of 2 in (200000)! is <200000)                               */
#define SIEVE_LIMIT 400000
/* maximum distinct primes needed (number of primes <= SIEVE_LIMIT)           */
#define MAX_PRIMES 30000

static bool init_done = false;

/* prime list and smallest prime factor array */
static uint32_t primes[MAX_PRIMES];
static size_t   primes_cnt = 0;
static uint32_t spf[SIEVE_LIMIT + 1];

/* exponent of each prime value in the current factorial */
static uint32_t exp_by_value[SIEVE_LIMIT + 1];
/* list of prime values that currently have positive exponent */
static uint32_t present_primes[MAX_PRIMES];
static size_t   present_cnt = 0;

/* cache for flattening the triangle */
static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;

/* current factorial n (i.e. the next value to multiply in the sequence) */
static uint32_t cur_fac_n = 2;

/* --------------------------------------------------------------------- */
/*  Sieve to initialise primes and spf                                     */
static void init_sieve(void)
{
    if (init_done) return;
    for (uint32_t i = 0; i <= SIEVE_LIMIT; ++i) spf[i] = 0;
    for (uint32_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if (primes_cnt < MAX_PRIMES) primes[primes_cnt++] = i;
            if ((uint64_t)i * (uint64_t)i <= SIEVE_LIMIT) {
                for (uint64_t j = (uint64_t)i * i; j <= SIEVE_LIMIT; j += i) {
                    if (spf[(uint32_t)j] == 0) spf[(uint32_t)j] = i;
                }
            }
        }
    }
    init_done = true;
}

/* --------------------------------------------------------------------- */
/*  Factorise a single integer and update exp_by_value and present list   */
static void factor_and_update(uint32_t val)
{
    uint32_t x = val;
    while (x > 1) {
        uint32_t p = spf[x];
        uint32_t cnt = 0;
        while (x % p == 0) {
            x /= p;
            ++cnt;
        }
        if (exp_by_value[p] == 0) {
            present_primes[present_cnt++] = p;
        }
        exp_by_value[p] += cnt;
    }
}

/* --------------------------------------------------------------------- */
/*  Push a value into the dynamic cache, expanding if necessary            */
static void cache_push(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
        seq_t *newp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!newp) return; /* allocation failure: silently ignore (not expected) */
        cache = newp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* --------------------------------------------------------------------- */
/*  Generate one row starting from the current factorial exponents        */
static void generate_row(void)
{
    /* current exponent list for the factorial */
    typedef struct { uint32_t prime; uint32_t exp; } Frag;
    static Frag cur[MAX_PRIMES];
    static Frag next[MAX_PRIMES];
    size_t cur_len = 0, next_len = 0;

    /* fill current vector from present primes */
    for (size_t i = 0; i < present_cnt; ++i) {
        uint32_t p = present_primes[i];
        uint32_t e = exp_by_value[p];
        if (e) {
            cur[cur_len].prime = p;
            cur[cur_len].exp = e;
            ++cur_len;
        }
    }

    while (true) {
        /* compute Omega for current */
        seq_t omega = 0;
        for (size_t i = 0; i < cur_len; ++i) omega += (seq_t)cur[i].exp;

        cache_push(omega);

        if (omega == 1) break;

        /* build frequency of exponents */
        typedef struct { uint32_t exp; uint32_t count; } ExpCnt;
        static ExpCnt freq[MAX_PRIMES];
        size_t freq_len = 0;

        for (size_t i = 0; i < cur_len; ++i) {
            uint32_t e = cur[i].exp;
            bool found = false;
            for (size_t j = 0; j < freq_len; ++j) {
                if (freq[j].exp == e) {
                    ++freq[j].count;
                    found = true;
                    break;
                }
            }
            if (!found) {
                freq[freq_len].exp = e;
                freq[freq_len].count = 1;
                ++freq_len;
            }
        }

        /* construct next vector using prime(e) and count */
        next_len = 0;
        for (size_t i = 0; i < freq_len; ++i) {
            uint32_t e = freq[i].exp;
            uint32_t count = freq[i].count;
            /* e is >=1, and e-1 < primes_cnt by construction            */
            uint32_t pouf = primes[e - 1];  /* prime(e) */
            next[next_len].prime = pouf;
            next[next_len].exp = count;
            ++next_len;
        }

        /* swap current and next */
        for (size_t i = 0; i < next_len; ++i) cur[i] = next[i];
        cur_len = next_len;
    }
}

/* --------------------------------------------------------------------- */
/*  Main public function                                                  */
seq_t a(idx_t n)
{
    init_sieve();

    /* return cached value if already available */
    if ((size_t)n < cache_len) return cache[(size_t)n];

    /* generate rows until we have enough terms */
    while (cache_len <= (size_t)n) {
        factor_and_update(cur_fac_n);
        generate_row();
        ++cur_fac_n;   /* next factorial value for following row */
    }
    return cache[(size_t)n];
}
