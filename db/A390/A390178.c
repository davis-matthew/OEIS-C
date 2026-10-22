/*
 * A390178  Numbers that can be represented in more than one way as p^r + q^s
 *          with 4 distinct primes p < q, r, s.
 * https://oeis.org/A390178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 150, u64 n <= 150, u128 n <= 150,
 *            big any n
 * Helpers:   hash_u64, pow_u64_limit, seq_cmp_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Indirect computation: generates the sequence by enumerating all prime powers
   (with prime base and prime exponent, base≠exponent), forming unordered
   pairs that satisfy the distinctness requirement, counting the sums,
   keeping the sums that occur at least twice, sorting them, and returning
   the n‑th term (a(1) is the first listed term). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/hash_u64.h"
#include "../utils/pow_u64_limit.h"
#include "../utils/seq_cmp_local.h"

/* The build supplies definitions for seq_t and idx_t. */
/* typedef <unsigned> seq_t;      /* e.g. uint64_t or unsigned __int128 */
/* typedef uint64_t idx_t;        */

#define PRIME_LIMIT 1000000          /* primes up to 1 000 000 */
#define POW_LIMIT   100000000ULL     /* 1e8 – covers the first 10 000 terms */

/* ---------- prime sieve (simple) ---------- */
static uint32_t primes[80000];
static size_t primeCount = 0;

static void build_primes(void)
{
    if (primeCount != 0) return;
    bool *mul = (bool *)calloc(PRIME_LIMIT + 1, sizeof(bool));
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i) mul[i] = true;
    for (uint32_t i = 2; (uint64_t)i * i <= PRIME_LIMIT; ++i) {
        if (!mul[i]) continue;
        for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i) mul[j] = false;
    }
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i)
        if (mul[i]) primes[primeCount++] = i;
    free(mul);
}

/* ---------- prime exponent list ---------- */
static const uint32_t expPrimes[] = {2,3,5,7,11,13,17,19,23,29,31};
static const size_t EXP_PRIME_COUNT =
    sizeof(expPrimes) / sizeof(expPrimes[0]);

/* ---------- prime power struct ---------- */
typedef struct {
    uint64_t val;   /* p^r */
    uint32_t base;  /* p */
    uint32_t exp;   /* r */
} PrimePow;

static PrimePow *primePowers = NULL;
static size_t ppCnt = 0, ppCap = 0;

/* ---------- generate prime powers ---------- */
static void generate_prime_powers(void)
{
    if (ppCnt != 0) return;
    ppCap = 1024;
    primePowers = (PrimePow *)malloc(ppCap * sizeof(PrimePow));
    for (size_t pi = 0; pi < primeCount; ++pi) {
        uint32_t p = primes[pi];
        for (size_t ei = 0; ei < EXP_PRIME_COUNT; ++ei) {
            uint32_t r = expPrimes[ei];
            if (r == p) continue;                 /* base and exponent distinct */
            uint64_t v;
            if (!pow_u64_limit((uint64_t)p, r, POW_LIMIT, &v)) break;
            if (ppCnt == ppCap) {
                ppCap <<= 1;
                primePowers = (PrimePow *)realloc(
                    primePowers, ppCap * sizeof(PrimePow));
            }
            primePowers[ppCnt++] = (PrimePow){v, p, r};
        }
    }
}

/* ---------- simple open addressing hash map ---------- */
static uint64_t *mapKeys = NULL;
static unsigned char *mapFlags = NULL;  /* 0 empty, 1 once, 2 duplicate */
static size_t mapCap = 0, mapSize = 0;
#define MAP_INITIAL_CAP (1u << 20)

static void map_init(void)
{
    mapCap = MAP_INITIAL_CAP;
    mapKeys = (uint64_t *)calloc(mapCap, sizeof(uint64_t));
    mapFlags = (unsigned char *)calloc(mapCap, sizeof(unsigned char));
}

static void map_resize(void)
{
    size_t newCap = mapCap << 1;
    uint64_t *newKeys = (uint64_t *)calloc(newCap, sizeof(uint64_t));
    unsigned char *newFlags = (unsigned char *)calloc(newCap, sizeof(unsigned char));
    if (!newKeys || !newFlags) return;
    for (size_t i = 0; i < mapCap; ++i) {
        if (mapFlags[i] == 1) {
            uint64_t k = mapKeys[i];
            size_t j = hash_u64(k) & (newCap - 1);
            while (newFlags[j]) j = (j + 1) & (newCap - 1);
            newKeys[j] = k;
            newFlags[j] = 1;
        }
    }
    free(mapKeys);
    free(mapFlags);
    mapKeys = newKeys;
    mapFlags = newFlags;
    mapCap = newCap;
}

static void map_insert(uint64_t key)
{
    if (mapSize > mapCap >> 1) map_resize();
    size_t i = hash_u64(key) & (mapCap - 1);
    while (1) {
        if (mapFlags[i] == 0) {                     /* empty */
            mapKeys[i] = key;
            mapFlags[i] = 1;
            ++mapSize;
            return;
        }
        if (mapFlags[i] != 2 && mapKeys[i] == key) {  /* second hit */
            mapFlags[i] = 2;
            ++mapSize;
            return;
        }
        i = (i + 1) & (mapCap - 1);
    }
}

/* ---------- sequence buffer ---------- */
static seq_t *seqBuf = NULL;
static size_t seqCap = 0, seqLen = 0;

static void seq_append(uint64_t val)
{
    if (seqLen == seqCap) {
        seqCap = seqCap ? seqCap << 1 : 16;
        seqBuf = (seq_t *)realloc(seqBuf, seqCap * sizeof(seq_t));
    }
    seqBuf[seqLen++] = (seq_t)val;
}

/* ---------- sequence preparation ---------- */
static bool sequence_prepared = false;

static void prepare_sequence(void)
{
    if (sequence_prepared) return;
    build_primes();
    generate_prime_powers();
    map_init();

    for (size_t i = 0; i < ppCnt; ++i) {
        const PrimePow *a = &primePowers[i];
        for (size_t j = i + 1; j < ppCnt; ++j) {
            const PrimePow *b = &primePowers[j];

            /* ensure p < q */
            if (a->base >= b->base) continue;

            /* all four primes must be distinct :
               a.base, a.exp, b.base, b.exp */
            if (a->base == b->exp) continue;
            if (a->exp  == b->base) continue;
            if (a->exp  == b->exp) continue;

            uint64_t sum = a->val + b->val;
            map_insert(sum);
        }
    }

    /* record unique sums (those seen at least twice) */
    for (size_t i = 0; i < mapCap; ++i)
        if (mapFlags[i] == 2) seq_append(mapKeys[i]);

    if (seqLen > 1) qsort(seqBuf, seqLen, sizeof(seq_t), seq_cmp_local);

    sequence_prepared = true;
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    prepare_sequence();
    if ((size_t)n <= seqLen) return seqBuf[n - 1];
    return 0;
}
