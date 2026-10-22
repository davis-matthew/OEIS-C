/*
 * A350371  Numbers with exactly 4 semiprime divisors.
 * https://oeis.org/A350371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = numbers with exactly 4 semiprime divisors A number n has
 *            4 semiprime divisors iff n = p^e * q * r (p, q, r distinct
 *            primes, e ≥ 2). Equivalently, the prime‑factorisation of n
 *            contains exactly three distinct primes and exactly one of them
 *            has exponent at least 2
 * Range:     u8 n <= 16, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>


#if defined(_MSC_VER) && !defined(_WIN64)
typedef unsigned __int64 uint64_t;
typedef uint64_t idx_t;
#else
# include <inttypes.h>
#endif

/* Maximum limit for pre‑sieving.
   The 10 000‑th term is well below 10 000 000, so this covers all
   queries within the time limit. */
static const uint32_t LIMIT = 10000000;

/* Smallest prime factor table.  Allocated lazily on first call. */
static uint32_t *spf = NULL;
/* Sequence array – holds the generated terms. */
static seq_t  *seq  = NULL;
static size_t  seq_len = 0;
static size_t  seq_cap = 0;
/* Last processed integer (≥1).  The starting point is 1. */
static uint32_t processed = 1;
static int initialized = 0;

/* ------------------------------------------------------------------ */
/*  Initialise the smallest‑prime‑factor array and the sequence buffer. */
static void init(void)
{
    if (initialized) return;
    initialized = 1;

    /* Allocate and fill SPF table using a simple sieve. */
    spf = (uint32_t *) calloc(LIMIT + 1, sizeof(uint32_t));
    if (!spf) exit(1);

    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {               /* i is prime */
            spf[i] = i;
            if ((uint64_t)i * i <= LIMIT) {
                for (uint32_t j = i * i; j <= LIMIT; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }

    /* Allocate initial buffer for generated sequence terms. */
    seq_cap = 4096;
    seq = (seq_t *) malloc(seq_cap * sizeof(seq_t));
    if (!seq) exit(1);
    seq_len = 0;
}

/* ------------------------------------------------------------------ */
/*  Generate terms until we have at least n terms in the sequence. */
static void generate_upto(idx_t n)
{
    init();

    while (seq_len < n) {
        ++processed;
        if (processed > LIMIT) {
            /* If we run out of pre‑sieved range, stop – the bound
               is sufficiently large for n ≤ 10 000. */
            break;
        }

        uint32_t temp = processed;
        int distinct = 0;  /* number of distinct primes in factorisation */
        int squares  = 0;   /* number of primes with exponent ≥ 2 */

        while (temp > 1) {
            uint32_t p = spf[temp];
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            ++distinct;
            if (exp >= 2) ++squares;
        }

        /* Count of semiprime divisors = squares + C(distinct, 2) */
        if (squares + (distinct * (distinct - 1)) / 2 == 4) {
            if (seq_len == seq_cap) {
                seq_cap = seq_cap * 2 + 16;
                seq = (seq_t *) realloc(seq, seq_cap * sizeof(seq_t));
                if (!seq) exit(1);
            }
            seq[seq_len++] = (seq_t)processed;
        }
    }
}

/* ------------------------------------------------------------------ */
/*  Public entry point. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for 0, return 0 */
    generate_upto(n);
    return seq[n - 1];
}
