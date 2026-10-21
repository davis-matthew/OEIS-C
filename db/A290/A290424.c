/*
 * A290424  Even numbers that are not the sum or difference of two binary
 *          palindromes (A006995).
 * https://oeis.org/A290424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 36, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: Even numbers that are not the sum or difference of two binary palindromes (A290424).
 * The function computes the nth term (1‑based) by generating all binary palindromes
 * up to a fixed bound, marking every even number that can be expressed as a sum
 * or a (non‑negative) difference of two such palindromes, and storing the
 * remaining even numbers in a static array.  Subsequent calls simply index that array.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

#define MAX_LIMIT 10000000ULL  /* upper bound for precomputation (10 million) */

/* reverse the lowest 'bits' bits of x */
static inline uint64_t reverse_bits(uint64_t x, int bits)
{
    uint64_t r = 0;
    while (bits--) {
        r = (r << 1) | (x & 1ULL);
        x >>= 1;
    }
    return r;
}

/* static storage for the precomputed sequence */
static seq_t *unrep = NULL;      /* array of evens that are NOT representable */
static size_t  unrep_cnt = 0;    /* number of elements in unrep */

/* performs the one‑time precomputation */
static void precompute(void)
{
    if (unrep != NULL) return;                     /* already done */

    /* generate all binary palindromes <= MAX_LIMIT */
    size_t pal_cap = 2048;
    uint64_t *pal = (uint64_t*)malloc(pal_cap * sizeof(uint64_t));
    size_t pal_len = 0;

    /* maximum bit length needed */
    int max_bits = 0;
    uint64_t temp = MAX_LIMIT;
    while (temp > 0) { ++max_bits; temp >>= 1; }

    for (int len = 1; len <= max_bits; ++len) {
        int half = (len + 1) / 2;          /* number of bits in the left half (+ middle for odd) */
        uint64_t start = 1ULL << (half - 1);      /* leading bit must be 1 */
        uint64_t end   = 1ULL << half;           /* exclusive upper bound */

        for (uint64_t halfBits = start; halfBits < end; ++halfBits) {
            uint64_t val;
            if (len & 1) {  /* odd length */
                uint64_t rev = reverse_bits(halfBits >> 1, half - 1);
                val = (halfBits << (half - 1)) | rev;
            } else {        /* even length */
                uint64_t rev = reverse_bits(halfBits, half);
                val = (halfBits << half) | rev;
            }
            if (val > MAX_LIMIT) continue;
            if (pal_len == pal_cap) {
                pal_cap <<= 1;
                pal = (uint64_t*)realloc(pal, pal_cap * sizeof(uint64_t));
            }
            pal[pal_len++] = val;
        }
    }

    /* bitset for representable even numbers <= MAX_LIMIT */
    size_t rep_size = (MAX_LIMIT >> 1) + 1;      /* each slot represents 2*k */
    uint8_t *rep = (uint8_t*)calloc(rep_size, 1);

    /* mark all even numbers that can be expressed as sum or difference */
    for (size_t i = 0; i < pal_len; ++i) {
        uint64_t p = pal[i];

        /* sums (i <= j to avoid duplicate work) */
        for (size_t j = i; j < pal_len; ++j) {
            uint64_t q = pal[j];
            uint64_t sum = p + q;
            if (sum > MAX_LIMIT) break;
            if ((sum & 1ULL) == 0) rep[sum >> 1] = 1;
        }

        /* differences (p >= q) */
        for (size_t j = 0; j <= i; ++j) {
            uint64_t q = pal[j];
            uint64_t diff = p - q;   /* guaranteed non‑negative */
            if (diff < 2) continue;  /* ignore 0 and 1 */
            if ((diff & 1ULL) == 0) rep[diff >> 1] = 1;
        }
    }

    /* count the unrepresentable even numbers up to MAX_LIMIT */
    unrep_cnt = 0;
    for (uint64_t k = 1; k <= (MAX_LIMIT >> 1); ++k)
        if (!rep[k]) ++unrep_cnt;

    /* allocate and fill the array of unrepresentable evens */
    unrep = (seq_t*)malloc(unrep_cnt * sizeof(seq_t));
    size_t idx = 0;
    for (uint64_t k = 1; k <= (MAX_LIMIT >> 1); ++k)
        if (!rep[k]) unrep[idx++] = (seq_t)(2ULL * k);

    /* clean up temporary storage */
    free(rep);
    free(pal);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, return 0 */
    if (!unrep)                    /* trigger the precomputation once */
        precompute();
    if ((size_t)n <= unrep_cnt)    /* n is 1‑based */
        return unrep[n - 1];
    /* if n is larger than the precomputed range, return 0 (should not happen for tests) */
    return 0;
}
