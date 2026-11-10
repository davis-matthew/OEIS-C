/*
 * A357993  a(n) is the unique k such that A357961(k) = 2^n.
 * https://oeis.org/A357993
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the index k at which A357961(k) = 2^n. The code simulates
 *            A357961 (a(1) = 1, a(k+1) = the j-th unused positive integer, j
 *            = popcount(a(k))) one aligned block of 4096 values at a time.
 *            Each block's effect is memoised and keyed by the block's
 *            relative state
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Validated: partial, pass, 34 terms through n = 33 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define A357993_BB 12
#define A357993_BS (1 << A357993_BB)
#define A357993_OFFB 256
#define A357993_HBITS 20
#define A357993_MAXENT (1u << 21)
#define A357993_MAXPOOL (1u << 24)

static uint64_t A357993_U[128];   /* unused values below the front, sorted */
static int A357993_ucnt = 0;
static uint64_t A357993_f = 0;    /* front: largest value used so far */
static int A357993_p = 0;         /* popcount of last generated value */
static uint64_t A357993_nextH = 0;
static bool A357993_init = false;
static uint64_t A357993_found[64];
static bool A357993_has[64];

typedef struct {
    uint64_t key;
    int32_t next;
    uint32_t data;
    uint8_t nreads, cnt;
    int16_t fr;
    uint8_t p;
} A357993_ent;

static A357993_ent *A357993_E = NULL;
static uint32_t A357993_ne = 0, A357993_ecap = 0;
static int16_t *A357993_pool = NULL;
static uint32_t A357993_np = 0, A357993_pcap = 0;
static int32_t *A357993_head = NULL;

static int A357993_pc(uint64_t x)
{
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcountll(x);
#else
    int c = 0;
    while (x) { x &= x - 1; ++c; }
    return c;
#endif
}

static void A357993_record(uint64_t v)
{
    if (v != 0 && (v & (v - 1)) == 0) {
        int e = 0;
        while (((uint64_t)1 << e) != v) ++e;
        if (!A357993_has[e]) {
            A357993_has[e] = true;
            A357993_found[e] = A357993_f - (uint64_t)A357993_ucnt;
        }
    }
}

/* step-by-step simulation until the next front advance would reach >= lim */
static void A357993_direct_block(uint64_t lim)
{
    for (;;) {
        int k = A357993_p;
        uint64_t v;
        if (k <= A357993_ucnt) {
            v = A357993_U[k - 1];
            memmove(&A357993_U[k - 1], &A357993_U[k],
                    (size_t)(A357993_ucnt - k) * sizeof(uint64_t));
            --A357993_ucnt;
        } else {
            int s = k - A357993_ucnt - 1;
            uint64_t nx = A357993_f + (uint64_t)s + 1;
            if (nx >= lim) break;
            for (int j = 1; j <= s; ++j)
                A357993_U[A357993_ucnt++] = A357993_f + (uint64_t)j;
            A357993_f = nx;
            v = nx;
        }
        A357993_p = A357993_pc(v);
        A357993_record(v);
    }
}

static void A357993_apply(uint64_t H, const int16_t *codes, int cnt, int fr, int p)
{
    uint64_t tmp[128];
    for (int c = 0; c < cnt; ++c) {
        int code = codes[c];
        if (code < A357993_OFFB) tmp[c] = A357993_U[code];
        else tmp[c] = (uint64_t)((int64_t)H + (int64_t)(code - A357993_OFFB));
    }
    memcpy(A357993_U, tmp, (size_t)cnt * sizeof(uint64_t));
    A357993_ucnt = cnt;
    A357993_f = (uint64_t)((int64_t)H + (int64_t)fr);
    A357993_p = p;
}

static void A357993_memo_block(uint64_t H)
{
    uint64_t hi = H >> A357993_BB;
    int h = A357993_pc(hi), hp = A357993_pc(hi - 1);
    int fr0 = (int)((int64_t)A357993_f - (int64_t)H);
    int L = A357993_ucnt, p0 = A357993_p;
    int q[128];
    for (int i = 0; i < L; ++i) q[i] = A357993_pc(A357993_U[i]);

    uint64_t key = (uint64_t)h | ((uint64_t)hp << 7) | ((uint64_t)(fr0 + 128) << 14)
                 | ((uint64_t)p0 << 22) | ((uint64_t)L << 29);
    uint32_t hsh = (uint32_t)((key * 0x9E3779B97F4A7C15ULL) >> (64 - A357993_HBITS));

    for (int32_t i = A357993_head[hsh]; i >= 0; i = A357993_E[i].next) {
        A357993_ent *en = &A357993_E[i];
        if (en->key != key) continue;
        const int16_t *d = A357993_pool + en->data;
        bool ok = true;
        for (int r = 0; r < en->nreads; ++r)
            if (q[d[2 * r]] != d[2 * r + 1]) { ok = false; break; }
        if (ok) {
            A357993_apply(H, d + 2 * en->nreads, en->cnt, en->fr, en->p);
            return;
        }
    }

    /* miss: simulate in relative coordinates */
    int16_t ent[160];
    int16_t rd[160];
    int cnt = L, nr = 0, fr = fr0, pp = p0;
    for (int i = 0; i < L; ++i) ent[i] = (int16_t)i;
    for (;;) {
        int k = pp;
        if (k <= cnt) {
            int code = ent[k - 1];
            memmove(&ent[k - 1], &ent[k], (size_t)(cnt - k) * sizeof(int16_t));
            --cnt;
            if (code < A357993_OFFB) {
                int qq = q[code];
                rd[2 * nr] = (int16_t)code;
                rd[2 * nr + 1] = (int16_t)qq;
                ++nr;
                pp = qq;
            } else {
                int x = code - A357993_OFFB;
                pp = x >= 0 ? h + A357993_pc((uint64_t)x)
                            : hp + A357993_pc((uint64_t)(A357993_BS + x));
            }
        } else {
            int s = k - cnt - 1;
            int nx = fr + s + 1;
            if (nx >= A357993_BS) break;
            for (int j = 1; j <= s; ++j)
                ent[cnt++] = (int16_t)(fr + j + A357993_OFFB);
            fr = nx;
            pp = nx >= 0 ? h + A357993_pc((uint64_t)nx)
                         : hp + A357993_pc((uint64_t)(A357993_BS + nx));
        }
    }

    uint32_t need = (uint32_t)(2 * nr + cnt);
    if (A357993_ne < A357993_MAXENT && A357993_np + need <= A357993_MAXPOOL) {
        bool ok = true;
        if (A357993_ne >= A357993_ecap) {
            uint32_t nc = A357993_ecap ? A357993_ecap * 2 : 4096;
            A357993_ent *t = (A357993_ent *)realloc(A357993_E, (size_t)nc * sizeof(A357993_ent));
            if (t) { A357993_E = t; A357993_ecap = nc; } else ok = false;
        }
        if (ok && A357993_np + need > A357993_pcap) {
            uint32_t nc = A357993_pcap ? A357993_pcap : 65536;
            while (nc < A357993_np + need) nc *= 2;
            int16_t *t = (int16_t *)realloc(A357993_pool, (size_t)nc * sizeof(int16_t));
            if (t) { A357993_pool = t; A357993_pcap = nc; } else ok = false;
        }
        if (ok) {
            int16_t *d = A357993_pool + A357993_np;
            memcpy(d, rd, (size_t)(2 * nr) * sizeof(int16_t));
            memcpy(d + 2 * nr, ent, (size_t)cnt * sizeof(int16_t));
            A357993_ent *en = &A357993_E[A357993_ne];
            en->key = key;
            en->next = A357993_head[hsh];
            en->data = A357993_np;
            en->nreads = (uint8_t)nr;
            en->cnt = (uint8_t)cnt;
            en->fr = (int16_t)fr;
            en->p = (uint8_t)pp;
            A357993_head[hsh] = (int32_t)A357993_ne;
            ++A357993_ne;
            A357993_np += need;
        }
    }
    A357993_apply(H, ent, cnt, fr, pp);
}

seq_t a(idx_t n)
{
    if (n > 62) return 0;
    unsigned e = (unsigned)n;
    if (!A357993_init) {
        A357993_f = 1;
        A357993_p = 1;
        A357993_ucnt = 0;
        A357993_has[0] = true;
        A357993_found[0] = 1;
        A357993_nextH = 0;
        A357993_head = (int32_t *)malloc(sizeof(int32_t) << A357993_HBITS);
        if (A357993_head)
            memset(A357993_head, 0xFF, sizeof(int32_t) << A357993_HBITS);
        A357993_init = true;
    }
    while (!A357993_has[e]) {
        uint64_t H = A357993_nextH;
        bool direct = H < 2 * (uint64_t)A357993_BS || (H & (H - 1)) == 0
                      || A357993_head == NULL;
        for (int i = 0; i < A357993_ucnt && !direct; ++i) {
            uint64_t v = A357993_U[i];
            if ((v & (v - 1)) == 0) direct = true;
        }
        if (direct) A357993_direct_block(H + A357993_BS);
        else A357993_memo_block(H);
        A357993_nextH = H + A357993_BS;
    }
    return (seq_t)A357993_found[e];
}
