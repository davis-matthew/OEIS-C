/*
 * A242348  a(n) = A242251(n)/n.
 * https://oeis.org/A242348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A242251(n)/n, where A242251(n) is found by search: the
 *            smallest multiple of n not equal to any earlier term, sum
 *            a(i)+a(j), or |a(i)-a(j)| (i, j < n)
 * Range:     u8 n <= 20, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 91 terms through n = 899 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/mem.h"

static seq_t *A242251_terms = NULL;
static size_t A242251_cap = 0;

static unsigned char *A242251_bits = NULL;
static size_t A242251_bits_len = 0;

static void A242251_ensure_terms(size_t idx)
{
    if (idx <= A242251_cap) return;
    size_t new_cap = A242251_cap ? A242251_cap << 1 : 128;
    while (new_cap < idx) new_cap <<= 1;
    seq_t *nt = mem_new(seq_t, new_cap + 1);
    if (!nt) abort();
    if (A242251_terms) {
        mem_copy(nt, A242251_terms, (A242251_cap + 1) * sizeof(seq_t));
        mem_free(A242251_terms);
    }
    A242251_terms = nt;
    A242251_cap = new_cap;
}

static void A242251_ensure_bits(size_t idx)
{
    if (idx < A242251_bits_len) return;
    size_t new_bits = A242251_bits_len ? A242251_bits_len << 1 : 64;
    while (new_bits <= idx) new_bits <<= 1;
    size_t new_bytes = (new_bits + 7) / 8;
    unsigned char *tmp = (unsigned char *)realloc(A242251_bits, new_bytes);
    if (!tmp) abort();
    size_t old_bytes = A242251_bits_len / 8;
    if (new_bytes > old_bytes) memset(tmp + old_bytes, 0, new_bytes - old_bytes);
    A242251_bits = tmp;
    A242251_bits_len = new_bits;
}

static void A242251_set(size_t idx)
{
    A242251_ensure_bits(idx);
    A242251_bits[idx >> 3] |= (unsigned char)(1u << (idx & 7));
}

static bool A242251_is_banned(size_t idx)
{
    if (idx >= A242251_bits_len) return false;
    return (A242251_bits[idx >> 3] >> (idx & 7)) & 1u;
}

static seq_t A242251_a(idx_t n)
{
    if (n == 0) return 0;
    A242251_ensure_terms((size_t)n);
    static idx_t computed = 0;
    while (computed < n) {
        ++computed;
        idx_t cur = computed;
        seq_t cand = (seq_t)cur;
        while (A242251_is_banned((size_t)cand))
            cand += (seq_t)cur;
        A242251_terms[cur] = cand;
        A242251_set((size_t)cand);
        for (idx_t j = 1; j < cur; ++j) {
            seq_t sum = A242251_terms[j] + A242251_terms[cur];
            A242251_set((size_t)sum);
            seq_t diff = (A242251_terms[j] > A242251_terms[cur])
                         ? A242251_terms[j] - A242251_terms[cur]
                         : A242251_terms[cur] - A242251_terms[j];
            A242251_set((size_t)diff);
        }
        seq_t self_sum = A242251_terms[cur] + A242251_terms[cur];
        A242251_set((size_t)self_sum);
    }
    return A242251_terms[(size_t)n];
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    return A242251_a(n) / (seq_t)n;
}
