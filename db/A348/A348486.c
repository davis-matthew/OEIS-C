/*
 * A348486  a(n) = A348485(n) * A348485(n+1).
 * https://oeis.org/A348486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A348485(n) * A348485(n+1); A348485 built by greedy search
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 92 terms through n = 9976 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

typedef struct {
    seq_t prod;
    int  count;
} A348485_Pair;

static seq_t *A348485_seq = NULL;
static size_t A348485_len = 0;
static size_t A348485_cap = 0;

static A348485_Pair *A348485_pairs = NULL;
static size_t A348485_plen = 0;
static size_t A348485_pcap = 0;

static void A348485_seq_ensure(size_t need)
{
    if (need <= A348485_cap) return;
    size_t new_cap = A348485_cap ? A348485_cap << 1 : 16;
    while (new_cap < need) new_cap <<= 1;
    A348485_seq = mem_resize(seq_t, A348485_seq, new_cap);
    A348485_cap = new_cap;
}

static void A348485_pairs_ensure(size_t need)
{
    if (need <= A348485_pcap) return;
    size_t new_cap = A348485_pcap ? A348485_pcap << 1 : 16;
    while (new_cap < need) new_cap <<= 1;
    A348485_pairs = mem_resize(A348485_Pair, A348485_pairs, new_cap);
    A348485_pcap = new_cap;
}

static size_t A348485_find_pair(seq_t prod)
{
    for (size_t i = 0; i < A348485_plen; ++i) {
        if (A348485_pairs[i].prod == prod) return i;
    }
    return (size_t)-1;
}

static int A348485_get_pair_count(seq_t prod)
{
    size_t idx = A348485_find_pair(prod);
    return (idx == (size_t)-1) ? 0 : A348485_pairs[idx].count;
}

static void A348485_inc_pair(seq_t prod)
{
    size_t idx = A348485_find_pair(prod);
    if (idx != (size_t)-1) {
        A348485_pairs[idx].count++;
    } else {
        A348485_pairs_ensure(A348485_plen + 1);
        A348485_pairs[A348485_plen].prod = prod;
        A348485_pairs[A348485_plen].count = 1;
        A348485_plen++;
    }
}

static seq_t A348485_a(idx_t n)
{
    if (n == 0) return 0;
    if (A348485_len < (size_t)n) {
        A348485_seq_ensure((size_t)n);
        while (A348485_len < (size_t)n) {
            if (A348485_len == 0) {
                A348485_seq[0] = (seq_t)1;
                A348485_len = 1;
                continue;
            }
            seq_t prev = A348485_seq[A348485_len - 1];
            seq_t cand = (seq_t)1;
            for (;;) {
                seq_t prod = prev * cand;
                int used = A348485_get_pair_count(prod);
                if ((seq_t)used < prod) {
                    if (A348485_len >= 2 && A348485_seq[A348485_len - 2] == cand) {
                        cand++;
                        continue;
                    }
                    A348485_seq[A348485_len] = cand;
                    A348485_inc_pair(prod);
                    break;
                }
                cand++;
            }
            A348485_len++;
        }
    }
    return A348485_seq[(size_t)n - 1];
}

seq_t a(idx_t n)
{
    seq_t x = A348485_a(n);
    seq_t y = A348485_a(n + 1);
    return x * y;
}
