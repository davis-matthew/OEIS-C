/*
 * A331215  Lexicographically earliest sequence of distinct positive integers
 *          such that four successive digits are always distinct.
 * https://oeis.org/A331215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   A331215 is defined via a greedy construction. * Let S be the
 *            string obtained by concatenating the terms. * At each step we
 *            choose the smallest unused positive integer k * such that
 *            appending decimal(k) to S keeps the property that * any four
 *            consecutive digits of S are distinct. * * The algorithm...
 * Range:     u8 n <= 102, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- global state ---------- */
static seq_t *cache      = NULL;   /* cache of produced terms   */
static size_t cache_len  = 0;      /* number of cached terms    */
static size_t cache_cap  = 0;      /* cached capacity          */

static uint64_t *used_bits = NULL; /* bitset: used_bits[i] |= 1ULL<<j  -> number (i*64+j) used */
static size_t used_words  = 0;     /* number of 64‑bit words allocated */

/* last up to 3 digits of the entire concatenated string */
static uint8_t tail[3];
static size_t tail_len = 0;         /* 0..3 */

/* ---------- bitset helpers ---------- */
static void ensure_used_bits(idx_t n)
{
    size_t needed = (n >> 6) + 1;        /* number of 64‑bit words required */
    if (needed > used_words) {
        size_t new_words = used_words ? used_words << 1 : 8;
        if (new_words < needed) new_words = needed;
        uint64_t *tmp = (uint64_t *)realloc(used_bits, new_words * sizeof(uint64_t));
        if (!tmp) abort();                 /* out of memory */
        used_bits = tmp;
        for (size_t i = used_words; i < new_words; ++i) used_bits[i] = 0;
        used_words = new_words;
    }
}

static void set_used(idx_t n)
{
    ensure_used_bits(n);
    used_bits[n >> 6] |= 1ULL << (n & 63);
}

static bool is_used(idx_t n)
{
    if ((n >> 6) >= used_words) return false;
    return (used_bits[n >> 6] >> (n & 63)) & 1ULL;
}

/* ---------- helper: digits of a uint64_t in MSD order ---------- */
static size_t digits_of_uint64(uint64_t x, uint8_t *buf, size_t maxlen)
{
    /* handle the case x == 0, though not used here */
    if (x == 0) {
        if (maxlen > 0) { buf[0] = 0; return 1; }
        return 0;
    }
    size_t len = 0;
    while (x != 0) {
        if (len >= maxlen) return 0; /* overflow protection */
        buf[len++] = (uint8_t)(x % 10);
        x /= 10;
    }
    /* reverse to get MSD first */
    for (size_t i = 0; i < len / 2; ++i) {
        uint8_t tmp = buf[i];
        buf[i] = buf[len - 1 - i];
        buf[len - 1 - i] = tmp;
    }
    return len;
}

/* ---------- candidate validity check ---------- */
static bool candidate_valid(uint64_t cand)
{
    uint8_t digits[64];
    size_t dlen = digits_of_uint64(cand, digits, sizeof(digits));
    if (dlen == 0) return false; /* overflow, shouldn't happen */

    uint8_t cur[3];
    size_t clen = tail_len;
    for (size_t i = 0; i < tail_len; ++i) cur[i] = tail[i];

    for (size_t i = 0; i < dlen; ++i) {
        uint8_t d = digits[i];
        if (clen < 3) {
            cur[clen++] = d;
            continue;
        }
        if (d == cur[0] || d == cur[1] || d == cur[2]) return false;
        /* shift left */
        cur[0] = cur[1];
        cur[1] = cur[2];
        cur[2] = d;
    }
    return true;
}

/* ---------- update global tail with a new term ---------- */
static void update_global_tail(uint64_t cand)
{
    uint8_t digits[64];
    size_t dlen = digits_of_uint64(cand, digits, sizeof(digits));

    for (size_t i = 0; i < dlen; ++i) {
        uint8_t d = digits[i];
        if (tail_len < 3) {
            tail[tail_len++] = d;
        } else {
            tail[0] = tail[1];
            tail[1] = tail[2];
            tail[2] = d;
        }
    }
}

/* ---------- generate sequence up to n terms ---------- */
static void generate_up_to(idx_t n)
{
    while (cache_len < n) {
        idx_t cand = 1;
        while (true) {
            if (!is_used(cand)) {
                if (candidate_valid((uint64_t)cand)) {
                    /* accept this candidate */
                    set_used(cand);
                    if (cache_cap <= cache_len) {
                        cache_cap = cache_cap ? cache_cap << 1 : 8;
                        seq_t *tmp = (seq_t *)realloc(cache,
                                                     cache_cap * sizeof(seq_t));
                        if (!tmp) abort();
                        cache = tmp;
                    }
                    cache[cache_len++] = (seq_t)cand;
                    update_global_tail(cand);
                    break;
                }
            }
            ++cand;
        }
    }
}

/* ---------- public function required by the problem ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined for offset 1, harmless */
    generate_up_to(n);
    return cache[n - 1];
}
