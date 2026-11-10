/*
 * A275158  Numbers n such that A003144(n) = floor(alpha*n) + 1, where alpha =
 *          1.839... is the positive real zero of x^3-x^2-x-1.
 * https://oeis.org/A275158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search over m = 1, 2, ...; keep m with A003144(m) =
 *            floor(alpha*m) + 1, alpha = 1.839... the real root of
 *            x^3-x^2-x-1 (exact integer arithmetic on a 27-digit constant)
 * Range:     u8 none, u16 n <= 8, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static char *A275158_w = NULL;
static size_t A275158_len = 0, A275158_cap = 0, A275158_gen = 0;
static uint64_t *A275158_pos = NULL;
static size_t A275158_npos = 0, A275158_pcap = 0;

static void A275158_push(char c) {
    if (A275158_len >= A275158_cap) {
        size_t nc = A275158_cap ? A275158_cap * 2 : 1024;
        A275158_w = (char *)realloc(A275158_w, nc);
        A275158_cap = nc;
    }
    A275158_w[A275158_len++] = c;
    if (c == 'a') {
        if (A275158_npos >= A275158_pcap) {
            size_t nc = A275158_pcap ? A275158_pcap * 2 : 1024;
            A275158_pos = (uint64_t *)realloc(A275158_pos, nc * sizeof(uint64_t));
            A275158_pcap = nc;
        }
        A275158_pos[A275158_npos++] = (uint64_t)A275158_len;
    }
}

static void A275158_ensure(uint64_t n) {
    if (A275158_len == 0) {
        A275158_push('a');
        A275158_push('b');
        A275158_gen = 1;
    }
    while ((uint64_t)A275158_npos < n) {
        char c = A275158_w[A275158_gen++];
        if (c == 'a') {
            A275158_push('a');
            A275158_push('b');
        } else if (c == 'b') {
            A275158_push('a');
            A275158_push('c');
        } else {
            A275158_push('a');
        }
    }
}

static uint64_t *A275158_found = NULL;
static size_t A275158_nfound = 0, A275158_fcap = 0;
static uint64_t A275158_next = 1;

seq_t a(idx_t n) {
    const unsigned __int128 ALPHA = (unsigned __int128)1839286755214161132ULL * 1000000000ULL
                                    + 0; /* placeholder, replaced below */
    (void)ALPHA;
    /* alpha * 10^27 = 1839286755214161132551852564 */
    unsigned __int128 A = (unsigned __int128)1839286755214161132ULL;
    A = A * 1000000000ULL + 551852564ULL;
    unsigned __int128 SCALE = (unsigned __int128)1000000000ULL * 1000000000ULL * 1000000000ULL;
    while (A275158_nfound < (size_t)n) {
        uint64_t m = A275158_next++;
        A275158_ensure(m);
        uint64_t t = A275158_pos[m - 1];
        uint64_t fl = (uint64_t)(((unsigned __int128)m * A) / SCALE);
        if (t == fl + 1) {
            if (A275158_nfound >= A275158_fcap) {
                size_t nc = A275158_fcap ? A275158_fcap * 2 : 256;
                A275158_found = (uint64_t *)realloc(A275158_found, nc * sizeof(uint64_t));
                A275158_fcap = nc;
            }
            A275158_found[A275158_nfound++] = m;
        }
    }
    return (seq_t)A275158_found[n - 1];
}
