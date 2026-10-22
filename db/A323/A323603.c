/*
 * A323603  Smallest b > 1 not already in the sequence such that b^(c-1) == 1
 *          (mod c^2), i.e., c is a base-b "Wieferich pseudoprime", where c is
 *          the n-th composite number (A002808).
 * https://oeis.org/A323603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements OEIS A323603: for the n‑th composite number c,
   find the smallest integer base b > 1 not yet used such that
   b^(c‑1) ≡ 1 (mod c²).  The returned value a(n) is that base b. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"

static void ensure_used_capacity(size_t need_bytes);
static inline bool is_used(size_t b);
static inline void set_used(size_t b);

static void ensure_comp_capacity(size_t need);
static void ensure_a_capacity(size_t need);

/* cached results */
static seq_t *a_cache = NULL;
static size_t a_cap   = 0, a_len   = 0;

/* cached composite numbers */
static idx_t *comp_arr = NULL;
static size_t comp_cap = 0, comp_len = 0;
static idx_t next_comp_candidate = 4;

/* bitmap of bases already used */
static uint8_t *used_bits = NULL;
static size_t used_bytes  = 0;

/*--- bitmap helpers ----------------------------------------------------*/
static void ensure_used_capacity(size_t need_bytes)
{
    if (need_bytes <= used_bytes) return;
    size_t new_bytes = used_bytes ? used_bytes << 1U : 8;
    while (new_bytes < need_bytes) new_bytes <<= 1U;
    uint8_t *p = (uint8_t *)realloc(used_bits, new_bytes);
    if (p) {
        if (used_bits == NULL)
            memset(p, 0, new_bytes);
        else
            memset(p + used_bytes, 0, new_bytes - used_bytes);
    }
    used_bits  = p;
    used_bytes = new_bytes;
}

static inline bool is_used(size_t b)
{
    size_t byte = b >> 3;
    if (byte >= used_bytes) return false;
    return (used_bits[byte] >> (b & 7U)) & 1U;
}

static inline void set_used(size_t b)
{
    size_t byte = b >> 3;
    ensure_used_capacity(byte + 1);
    used_bits[byte] |= (1U << (b & 7U));
}

/*--- composite number cache -------------------------------------------*/
static void ensure_comp_capacity(size_t need)
{
    if (need <= comp_cap) return;
    size_t new_cap = comp_cap ? comp_cap << 1U : 8;
    while (new_cap < need) new_cap <<= 1U;
    idx_t *p = (idx_t *)realloc(comp_arr, new_cap * sizeof(idx_t));
    comp_arr = p;
    comp_cap = new_cap;
}

/*--- sequence cache ---------------------------------------------------*/
static void ensure_a_capacity(size_t need)
{
    if (need <= a_cap) return;
    size_t new_cap = a_cap ? a_cap << 1U : 8;
    while (new_cap < need) new_cap <<= 1U;
    seq_t *p = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    a_cache = p;
    a_cap   = new_cap;
}

/*--- main function -----------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined; return 0 */

    if (a_len < n) {               /* need more terms */
        for (idx_t cur = a_len + 1; cur <= n; ++cur) {
            /* fill comp_arr with first cur composites */
            while (comp_len < cur) {
                if (!is_prime(next_comp_candidate)) {
                    ensure_comp_capacity(comp_len + 1);
                    comp_arr[comp_len++] = next_comp_candidate;
                }
                ++next_comp_candidate;
            }
            idx_t c = comp_arr[cur - 1];
            seq_t mod_val = (seq_t)c * (seq_t)c;

            idx_t b = 2;
            for (;;) {
                if (!is_used((size_t)b)) {
                    seq_t r = mod_pow((seq_t)b, (uint64_t)(c - 1), mod_val);
                    if (r == (seq_t)1) {
                        set_used((size_t)b);
                        ensure_a_capacity(a_len + 1);
                        a_cache[a_len++] = (seq_t)b;
                        break;
                    }
                }
                ++b;
            }
        }
    }
    return a_cache[n - 1];
}
