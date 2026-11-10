/*
 * A342792  Positions in A342753 of words in which #0's - #1's is even.
 * https://oeis.org/A342792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th k with len(w(k)) even, where w(1)="0", w(k)=bit(k)
 *            w(c(k)), len(w(k)) = 1 + len(w(c(k))), len(w(0)) = 0 (#0 - #1
 *            even iff word length even); c(k)=(k-1)/3 if 3|k else
 *            (k-1)-floor((k-1)/3); computed by sieve-like scan
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 63 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static unsigned char *A342792_len = NULL;
static idx_t A342792_lcap = 0;
static idx_t A342792_k = 0;
static idx_t *A342792_pos = NULL;
static idx_t A342792_pcap = 0;
static idx_t A342792_pn = 0;

seq_t a(idx_t n)
{
    if (A342792_len == NULL) {
        A342792_lcap = 1024;
        A342792_len = (unsigned char *)malloc((size_t)A342792_lcap);
        A342792_pcap = 256;
        A342792_pos = (idx_t *)malloc((size_t)A342792_pcap * sizeof(idx_t));
        if (!A342792_len || !A342792_pos) exit(EXIT_FAILURE);
        A342792_len[0] = 0;
        A342792_k = 0;
        A342792_pn = 0;
    }
    while (A342792_pn < n) {
        idx_t k = A342792_k + 1;
        if (k >= A342792_lcap) {
            idx_t nc = A342792_lcap * 2;
            unsigned char *t = (unsigned char *)realloc(A342792_len, (size_t)nc);
            if (!t) exit(EXIT_FAILURE);
            A342792_len = t;
            A342792_lcap = nc;
        }
        unsigned char L;
        if (k == 1) {
            L = 1;
        } else {
            idx_t am = k - 1;
            idx_t tp = am / 3;
            idx_t child = (k % 3 == 0) ? tp : (am - tp);
            L = (unsigned char)(1 + A342792_len[child]);
        }
        A342792_len[k] = L;
        A342792_k = k;
        if ((L & 1) == 0) {
            if (A342792_pn >= A342792_pcap) {
                idx_t nc = A342792_pcap * 2;
                idx_t *t = (idx_t *)realloc(A342792_pos, (size_t)nc * sizeof(idx_t));
                if (!t) exit(EXIT_FAILURE);
                A342792_pos = t;
                A342792_pcap = nc;
            }
            A342792_pos[A342792_pn++] = k;
        }
    }
    return (seq_t)A342792_pos[n - 1];
}
